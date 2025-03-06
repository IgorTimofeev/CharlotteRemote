#include "pfd.h"
#include "../../../rc.h"
#include "../../theme.h"

namespace pizda {
	PFD::PFD() {
		setClipToBounds(true);
	}

	void PFD::renderCurrentValue(Renderer* renderer, const Bounds& bounds, int32_t centerY, float value, bool left) {
		int32_t y = centerY - currentValueHeight / 2;

		// Triangle
		renderer->renderFilledTriangle(
			Point(
				left ? bounds.getX2() - currentValueTriangleSize - 1 : bounds.getX() + currentValueTriangleSize - 1,
				y
			),
			Point(
				left ? bounds.getX2() : bounds.getX(),
				centerY
			),
			Point(
				left ? bounds.getX2() - currentValueTriangleSize - 1 : bounds.getX() + currentValueTriangleSize - 1,
				y + currentValueHeight - 1
			),
			&Theme::bg2
		);

		// Rect
		const auto& rectangleBounds = Bounds(
			left ? bounds.getX() : bounds.getX() + currentValueTriangleSize,
			y,
			bounds.getWidth() - currentValueTriangleSize,
			currentValueHeight
		);

		renderer->renderFilledRectangle(rectangleBounds, &Theme::bg2);

		// Text
		constexpr static const uint8_t textOffset = 0;

		const auto oldViewport = renderer->pushViewport(rectangleBounds);

		auto uintValue = (uint32_t) value;

		// Assuming 4 is "widest" digit
		const uint8_t maxDigitWidth = Theme::fontNormal.getCharWidth(L'4');

		int32_t x =
			left
			? bounds.getX2() - currentValueTriangleSize - textOffset
			: bounds.getX() + currentValueTriangleSize + textOffset + maxDigitWidth * getDigitCount(uintValue);

		y = y + currentValueHeight / 2 - Theme::fontNormal.getHeight() / 2;

		float integer;
		const auto fractional = std::modf(value, &integer);
		const int32_t scrolledY = y + (uint8_t) (fractional * (float) Theme::fontNormal.getHeight());

		const auto getAdjacentDigit = [&](uint8_t digit, bool plus) {
			return
				plus
				? (digit < 9 ? digit + 1 : 0)
				: (digit > 1 ? digit - 1 : 9);
		};

		const auto renderDigit = [&](int32_t digitY, uint8_t digit) {
			const wchar_t text = L'0' + digit;

			renderer->renderChar(
				Point(
					x - Theme::fontNormal.getCharWidth(text),
					digitY
				),
				&Theme::fontNormal,
				&Theme::fg1,
				text
			);
		};

		auto shouldScroll = true;
		uint8_t digit;

		do {
			digit = uintValue % 10;

			if (shouldScroll) {
				renderDigit(scrolledY - Theme::fontNormal.getHeight(), getAdjacentDigit(digit, true));
				renderDigit(scrolledY, digit);
				renderDigit(scrolledY + Theme::fontNormal.getHeight(), getAdjacentDigit(digit, false));
			}
			else {
				renderDigit(y, digit);
			}

			x -= maxDigitWidth;
			shouldScroll = digit == 9;
			uintValue /= 10;
		} while (uintValue > 0);

		renderer->popViewport(oldViewport);
	}

	void PFD::renderAutopilotValueIndicator(Renderer* renderer, const Point& point, bool left) {
		// Upper
		renderer->renderFilledRectangle(
			Bounds(
				point.getX(),
				point.getY(),
				autopilotIndicatorWidth,
				autopilotIndicatorTriangleVerticalMargin
			),
			&Theme::blue
		);

		// Lower
		renderer->renderFilledRectangle(
			Bounds(
				point.getX(),
				point.getY() + autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin,
				autopilotIndicatorWidth,
				autopilotIndicatorTriangleVerticalMargin
			),
			&Theme::blue
		);

		// Rect
		renderer->renderFilledRectangle(
			Bounds(
				left ? point.getX() + autopilotIndicatorTriangleWidth : point.getX(),
				point.getY() + autopilotIndicatorTriangleVerticalMargin,
				autopilotIndicatorRectangleWidth,
				autopilotIndicatorTriangleHeight
			),
			&Theme::blue
		);

		// Upper triangle
		renderer->renderFilledTriangle(
			Point(
				left ? point.getX() : point.getX() + autopilotIndicatorRectangleWidth,
				point.getY() + autopilotIndicatorTriangleVerticalMargin
			),
			Point(
				left ? point.getX() + autopilotIndicatorTriangleWidth - 1 : point.getX() + autopilotIndicatorWidth - 1,
				point.getY() + autopilotIndicatorTriangleVerticalMargin
			),
			Point(
				left ? point.getX() + autopilotIndicatorTriangleWidth - 1 : point.getX() + autopilotIndicatorRectangleWidth - 1,
				point.getY() + autopilotIndicatorHeightHalf
			),
			&Theme::blue
		);

		// Lower triangle
		renderer->renderFilledTriangle(
			Point(
				left ? point.getX() + autopilotIndicatorTriangleWidth - 1 : point.getX() + autopilotIndicatorRectangleWidth,
				point.getY() + autopilotIndicatorHeightHalf
			),
			Point(
				left ? point.getX() + autopilotIndicatorTriangleWidth - 1 : point.getX() + autopilotIndicatorWidth - 1,
				point.getY() + autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin - 1
			),
			Point(
				left ? point.getX() : point.getX() + autopilotIndicatorRectangleWidth - 1,
				point.getY() + autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin - 1
			),
			&Theme::blue
		);
	}

	void PFD::renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, uint16_t autopilotValue, bool left) {
		if (autopilotValue == 0)
			return;

		renderAutopilotValueIndicator(
			renderer,
			Point(
				left ? bounds.getX2() + 1 - autopilotIndicatorWidth : bounds.getX(),
				centerY + (int32_t) ((currentValue - (float) autopilotValue) * (float) unitPixels / (float) unitStep) - autopilotIndicatorHeightHalf
			),
			left
		);
	}

	void PFD::renderTrendArrow(Renderer* renderer, int32_t x, int32_t y, uint8_t unitStep, uint16_t unitPixels, float value) {
		const auto length = (int32_t) ((float) unitPixels * value / (float) unitStep);

		if (abs(length) < 10)
			return;

		const uint8_t arrowSize = 3;

		const auto yArrow = y - length - arrowSize;
		const auto yMin = std::min(y, yArrow);
		const auto yMax = std::max(y, yArrow);

		renderer->renderVerticalLine(
			Point(x, yMin),
			yMax - yMin,
			&Theme::purple
		);

		renderer->renderFilledTriangle(
			Point(
				x - arrowSize,
				yArrow
			),
			Point(
				x,
				yArrow + (length > 0 ? -arrowSize : arrowSize)
			),
			Point(
				x + arrowSize,
				yArrow
			),
			&Theme::purple
		);
	}

	void PFD::renderSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto centerY = bounds.getYCenter();

		renderer->renderFilledRectangle(bounds, &Theme::bg1);

		float speed = rc.getAirspeedInterpolator().getValue();

		// Bars
		const auto barX = bounds.getX2() + 1 - speedBarSize;

		const auto renderBar = [&](int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color* color) {
			int32_t fromY = centerY + (int32_t) std::ceilf(speed * (float) speedStepPixels - (float) fromSpeed * (float) speedStepPixels);
			int32_t height = (toSpeed - fromSpeed) * speedStepPixels;

			renderer->renderFilledRectangle(
				Bounds(
					x,
					fromY - height,
					width,
					height
				),
				color
			);
		};

		renderBar(
			barX,
			speedBarSize,
			0,
			speedFlapsMin,
			&Theme::red
		);

		renderBar(
			barX,
			speedBarSize,
			speedFlapsMin,
			speedTurbulentMin,
			&Theme::fg1
		);

		renderBar(
			barX,
			speedBarSize / 2,
			speedTurbulentMin,
			speedFlapsMax,
			&Theme::fg1
		);

		renderBar(
			barX + speedBarSize / 2,
			speedBarSize / 2,
			speedTurbulentMin,
			speedFlapsMax,
			&Theme::greenSpeed
		);

		renderBar(
			barX,
			speedBarSize,
			speedFlapsMax,
			speedTurbulentMax,
			&Theme::greenSpeed
		);

		renderBar(
			barX,
			speedBarSize,
			speedTurbulentMax,
			speedSmoothMax,
			&Theme::yellow
		);

		renderBar(
			barX,
			speedBarSize,
			speedSmoothMax,
			speedSmoothMax * 2,
			&Theme::red
		);

		// Lines
		float snapped = speed / (float) speedStepUnits;
		float snappedInteger = std::floorf(snapped);
		float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) speedStepPixels);
		auto altitudeYFullLines = (int32_t) std::ceil((float) y / (float) speedStepPixels);
		y = y - altitudeYFullLines * speedStepPixels;

		int32_t lineValue = (int32_t) (snappedInteger + 1) * speedStepUnits + altitudeYFullLines * speedStepUnits;

		bool isBig;
		const Color* lineColor = &Theme::fg3;

		do {
			isBig = lineValue % speedStepUnitsBig == 0;

			if (isBig) {
				// Line
				renderer->renderHorizontalLine(
					Point(
						bounds.getX2() + 1 - speedBarSize - lineSizeBig,
						y
					),
					lineSizeBig,
					lineColor
				);

				// Text
				const auto& text = std::to_wstring(lineValue);

				renderer->renderString(
					Point(
						bounds.getX2() + 1 - speedBarSize - lineSizeBig - lineSizeTextOffset - Theme::fontNormal.getWidth(text),
						y - Theme::fontNormal.getHeight() / 2
					),
					&Theme::fontNormal,
					lineColor,
					text
				);
			}
			else {
				// Line
				renderer->renderHorizontalLine(
					Point(bounds.getX2() + 1 - speedBarSize - lineSizeSmall, y),
					lineSizeSmall,
					lineColor
				);
			}

			lineValue -= speedStepUnits;
			y += speedStepPixels;
		} while (y <= bounds.getY2() && lineValue >= 0);

		// Trend
		renderTrendArrow(
			renderer,
			bounds.getX2() - speedBarSize - lineSizeBig,
			centerY,
			speedStepUnits,
			speedStepPixels,
			rc.getAirspeedTrendInterpolator().getValue()
		);

		// Autopilot
		renderAutopilotValueIndicator(
			renderer,
			bounds,
			centerY,
			speedStepUnits,
			speedStepPixels,
			speed,
			rc.getSettings().autopilot.speed,
			true
		);

		// Current speed
		renderCurrentValue(
			renderer,
			bounds,
			centerY,
			speed,
			true
		);
	}

	void PFD::renderAircraftSymbol(Renderer* renderer, const Point& center) {
		const uint8_t aircraftSymbolWidth = 30;
		const uint8_t aircraftSymbolThickness = 2;
		const uint8_t aircraftSymbolHeight = 6;
		const uint8_t aircraftSymbolCenterOffset = 16;

		// Left
		renderer->renderFilledRectangle(
			Bounds(
				center.getX() - aircraftSymbolCenterOffset - aircraftSymbolThickness,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolThickness,
				aircraftSymbolHeight
			),
			&Theme::bg1
		);

		renderer->renderFilledRectangle(
			Bounds(
				center.getX() - aircraftSymbolCenterOffset - aircraftSymbolThickness - aircraftSymbolWidth,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolWidth,
				aircraftSymbolThickness
			),
			&Theme::bg1
		);

		// Right
		renderer->renderFilledRectangle(
			Bounds(
				center.getX() + aircraftSymbolCenterOffset,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolThickness,
				aircraftSymbolHeight
			),
			&Theme::bg1
		);

		renderer->renderFilledRectangle(
			Bounds(
				center.getX() + aircraftSymbolCenterOffset + aircraftSymbolThickness,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolWidth,
				aircraftSymbolThickness
			),
			&Theme::bg1
		);

		// Dot
		renderer->renderFilledRectangle(
			Bounds(
				center.getX() + aircraftSymbolThickness / 2 - aircraftSymbolThickness / 2,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolThickness,
				aircraftSymbolThickness
			),
			&Theme::bg1
		);
	}

	void PFD::renderSyntheticVisionBackground(
		Renderer* renderer,
		const Bounds& bounds,
		const Point& horizonLeft,
		const Point& horizonRight
	) {
		// Sky
		renderer->renderFilledRectangle(bounds, &Theme::sky);

		// Ground
		const auto groundMaxY = std::max(horizonLeft.getY(), horizonRight.getY());

		// Triangle
		renderer->renderFilledTriangle(
			horizonLeft.getY() < horizonRight.getY()
			? horizonLeft
			: horizonRight,
			Point(
				horizonLeft.getX(),
				groundMaxY
			),
			Point(
				horizonRight.getX(),
				groundMaxY
			),
			&Theme::ground
		);

		// Rectangle

		// Bottom
		if (groundMaxY < bounds.getY2()) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					groundMaxY,
					bounds.getWidth(),
					bounds.getY2() - groundMaxY + 1
				),
				&Theme::ground
			);
		}
		// Left
		else if (horizonLeft.getY() < bounds.getY() && horizonLeft.getX() > bounds.getX()) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY(),
					horizonLeft.getX() - bounds.getX(),
					bounds.getHeight()
				),
				&Theme::ground
			);
		}
			// Right
		else if (horizonRight.getY() < bounds.getY() && horizonRight.getX() < bounds.getX2()) {
			renderer->renderFilledRectangle(
				Bounds(
					horizonRight.getX(),
					bounds.getY(),
					bounds.getX2() - horizonRight.getX(),
					bounds.getHeight()
				),
				&Theme::ground
			);
		}
	}

	void PFD::renderPitchOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		float unfoldedFOVHeight,
		const Point& horizonLeft,
		const Point& horizonRight
	) {
		const auto viewport = renderer->pushViewport(bounds);

		const float lineAngleStepRad = toRadians(pitchOverlayAngleStep);
		const float linesInTotal = std::floorf(((float) M_PI_2) / lineAngleStepRad);
		const float linePixelStep = (float) unfoldedFOVHeight / linesInTotal;

		const auto& horizonVec = (Vector2F) (horizonRight - horizonLeft);
		const auto& horizonCenter = (Vector2F) horizonLeft + horizonVec / 2.0f;
		const auto& horizonVecNorm = horizonVec.normalize();
		const auto& horizonVecPerp = horizonVecNorm.perpendicular();

		Vector2F
			lineCenterVerticalPerp,
			lineVec;

		Point
			lineLeft,
			lineRight;

		const Color* color;

		for (int32_t lineAngleDeg = -90; lineAngleDeg <= 90; lineAngleDeg += pitchOverlayAngleStep) {
			color = lineAngleDeg >= 0 ? pitchOverlayColorGround : pitchOverlayColorSky;
			lineCenterVerticalPerp = horizonCenter + horizonVecPerp * ((float) lineAngleDeg / (float) pitchOverlayAngleStep * linePixelStep);

			lineVec = horizonVecNorm * (
				lineAngleDeg == 0
				? pitchOverlayLineMiddle
				: (
					lineAngleDeg % 10 == 0
					? pitchOverlayLineBig
					: pitchOverlayLineSmall
				)
			);

			lineLeft = (Point) (lineCenterVerticalPerp - lineVec);
			lineRight = (Point) (lineCenterVerticalPerp + lineVec);

			renderer->renderLine(
				lineLeft,
				lineRight,
				color
			);

			if (lineAngleDeg != 0 && lineAngleDeg % 10 == 0) {
				renderer->renderString(
					Point(
						lineRight.getX() + pitchOverlayTextOffset,
						lineRight.getY() - Theme::fontSmall.getHeight() / 2
					),
					&Theme::fontSmall,
					color,
					std::to_wstring(abs(lineAngleDeg))
				);
			}
		}

		renderer->popViewport(viewport);
	}

	void PFD::renderRollOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		float roll
	) {
		const auto radius = ((float) bounds.getWidth() - rollOverlayHorizontalMargin * 2.f) / 2.f;

		const auto& center = Point(
			bounds.getXCenter(),
			bounds.getY() + ((uint16_t) radius)
		);

		const auto& renderLine = [renderer, &roll, &radius, &center](int8_t angle, bool isBig) {
			const auto& vec = Vector2F(0, radius).rotate(toRadians(angle) - roll);
			const auto& lineTo = center - (Point) vec;
			const auto& lineFrom = lineTo + (Point) (vec.normalize() * (isBig ? rollOverlayLineBigLength : rollOverlayLineSmallLength));

			renderer->renderLine(
				lineTo,
				lineFrom,
				rollOverlayColor
			);
		};

		renderLine(-60, true);
		renderLine(-45, true);
		renderLine(-30, false);
		renderLine(-20, false);
		renderLine(-10, false);
		renderLine(0, true);
		renderLine(10, false);
		renderLine(20, false);
		renderLine(30, false);
		renderLine(45, true);
		renderLine(60, true);

		// Small triangle representing current roll
		renderer->renderFilledTriangle(
			Point(center.getX() - rollOverlayTriangleSize, bounds.getY()),
			Point(center.getX() + rollOverlayTriangleSize, bounds.getY()),
			Point(center.getX(), bounds.getY() + rollOverlayTriangleSize),
			rollOverlayColor
		);
	}

	void PFD::renderYawOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		float yaw
	) {
		const auto viewport = renderer->pushViewport(bounds);

		const auto centerX = bounds.getXCenter();

		float closestInteger;
		float closestFractional = modff(toDegrees(yaw) / yawOverlayAngleStepUnits, &closestInteger);
		closestInteger *= yawOverlayAngleStepUnits;

		const uint8_t fullCount = (uint8_t) std::ceilf((float) (centerX - bounds.getX()) / yawOverlayAngleStepPixels) + 1;
		int32_t x = centerX - fullCount * yawOverlayAngleStepPixels - (int32_t) (closestFractional * (float) yawOverlayAngleStepPixels);
		auto angle = (int16_t) (closestInteger - (float) (fullCount * yawOverlayAngleStepUnits));

		if (angle < 0)
			angle += 360;

		bool isBig;
		uint8_t lineLength;

		int32_t lineY;

		while (x <= bounds.getX2()) {
			isBig = angle % 10 == 0;
			lineLength = isBig ? yawOverlayLineBigLength : yawOverlayLineSmallLength;

			// Line
			lineY = bounds.getY2() - lineLength + 1;

			renderer->renderVerticalLine(
				Point(
					x,
					lineY
				),
				lineLength,
				yawOverlayColor
			);

			// Text
			if (isBig) {
				std::wstring text;

				switch (angle) {
					case 0:
						text = L'N';
						break;

					case 90:
						text = L'E';
						break;

					case 180:
						text = L'S';
						break;

					case 270:
						text = L'W';
						break;

					default:
						text = std::to_wstring(angle);
						break;
				}

				renderer->renderString(
					Point(
						x - Theme::fontSmall.getWidth(text) / 2,
						lineY - yawOverlayTextOffset - Theme::fontSmall.getHeight()
					),
					&Theme::fontSmall,
					yawOverlayColor,
					text
				);
			}

			x += yawOverlayAngleStepPixels;
			angle += yawOverlayAngleStepUnits;
		}

		// Small triangle representing current heading
		renderer->renderFilledTriangle(
			Point(centerX - yawOverlayTriangleSize, bounds.getY2()),
			Point(centerX + yawOverlayTriangleSize, bounds.getY2()),
			Point(centerX, bounds.getY2() - yawOverlayTriangleSize),
			yawOverlayColor
		);

		renderer->popViewport(viewport);
	}

	void PFD::renderSyntheticVision(Renderer* renderer, const Bounds& bounds) const {
		auto& rc = RC::getInstance();

		const auto& center = bounds.getCenter();

		const auto pitch = rc.getPitchInterpolator().getValue();
		const auto roll = rc.getRollInterpolator().getValue();
		const auto yaw = rc.getYawInterpolator().getValue();

		// value = [180 deg of unfolded full range view] / [FOV deg of camera viewport] * [viewport size in pixels]
		const float unfoldedFovWidth = (float) M_PI / _horizontalFov / 2 * (float) bounds.getWidth();
		const float unfoldedFovHeight = (float) M_PI / _verticalFov / 2 * (float) bounds.getHeight();

		const auto& horizonRollRotated = (Point) Vector2F(unfoldedFovWidth, 0).rotate(-roll);
		const auto& horizonPitchRotated = (Point) Vector2F(unfoldedFovHeight, 0).rotate(pitch);

		const auto& horizonLeft = Point(
			center.getX() - horizonRollRotated.getX(),
			center.getY() - horizonPitchRotated.getY() - horizonRollRotated.getY()
		);

		const auto& horizonRight = Point(
			center.getX() + horizonRollRotated.getX(),
			center.getY() - horizonPitchRotated.getY() + horizonRollRotated.getY()
		);

		// Background
		renderSyntheticVisionBackground(
			renderer,
			bounds,
			horizonLeft,
			horizonRight
		);

		// Roll overlay
		renderRollOverlay(
			renderer,
			bounds,
			roll
		);

		// Pitch overlay
		renderPitchOverlay(
			renderer,
			Bounds(
				bounds.getX(),
				bounds.getY() + pitchOverlayMarginTop,
				bounds.getWidth(),
				bounds.getHeight() - pitchOverlayMarginTop - yawOverlayHeight
			),
			unfoldedFovHeight,
			horizonLeft,
			horizonRight
		);

		// Yaw overlay
		renderYawOverlay(
			renderer,
			Bounds(
				bounds.getX(),
				bounds.getY2() - yawOverlayHeight + 1,
				bounds.getWidth(),
				yawOverlayHeight
			),
			yaw
		);

		// Bird
		renderAircraftSymbol(renderer, center);

//		// Temp blyad radio
//
//		swprintf(text, 255, L"RSSI: %.2f dBm", app.getTransceiver().getRssi());
//		renderer->renderText(
//			Point(bounds.getX() + 10, bounds.getY() + 10),
//			&Theme::font,
//			&Theme::fg1,
//			text
//		);
//
//		swprintf(text, 255, L"SNR: %.2f dB", app.getTransceiver().getSnr());
//		renderer->renderText(
//			Point(bounds.getX() + 10, bounds.getY() + 20),
//			&Theme::font,
//			&Theme::fg1,
//			text
//		);
	}

	void PFD::renderAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto centerY = bounds.getYCenter();
		auto x = bounds.getX();

		renderer->renderFilledRectangle(bounds, &Theme::bg1);

		float altitude = rc.getAltitudeInterpolator().getValue();
		float snapped = altitude / (float) altitudeStepUnits;
		float snappedInteger = std::floorf(snapped);
		float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) altitudeUnitPixels);
		auto yFullLines = (int32_t) std::ceilf((float) y / (float) altitudeUnitPixels);
		y = y - yFullLines * altitudeUnitPixels;

		int32_t lineValue = (int32_t) (snappedInteger + 1) * altitudeStepUnits + yFullLines * altitudeStepUnits;

		bool isBig;

		const Color* lineColor = &Theme::fg3;

		do {
			isBig = lineValue % altitudeStepUnitsBig == 0;

			if (isBig) {
				renderer->renderHorizontalLine(
					Point(x, y),
					lineSizeBig,
					lineColor
				);

				// Text
				renderer->renderString(
					Point(x + lineSizeBig + lineSizeTextOffset, y - Theme::fontNormal.getHeight() / 2),
					&Theme::fontNormal,
					lineColor,
					std::to_wstring(lineValue)
				);
			}
			else {
				renderer->renderHorizontalLine(
					Point(x, y),
					lineSizeSmall,
					lineColor
				);
			}

			lineValue -= altitudeStepUnits;
			y += altitudeUnitPixels;
		} while (y < bounds.getY2() && lineValue >= 0);

		// Ground
		if (y < bounds.getY2() && lineValue < 0) {
			const int8_t groundSpacing = 5;
			auto groundPoint1 = Point(x, y + groundSpacing);
			auto groundPoint2 = Point(x + groundSpacing, y);

			do {
				renderer->renderLine(
					groundPoint1,
					groundPoint2,
					&Theme::yellow
				);

				// 1
				if (groundPoint1.getY() < bounds.getY2()) {
					groundPoint1.setY(groundPoint1.getY() + groundSpacing);
				}
				else {
					groundPoint1.setX(groundPoint1.getX() + groundSpacing);
				}

				// 2
				if (groundPoint2.getX() < bounds.getX2()) {
					groundPoint2.setX(groundPoint2.getX() + groundSpacing);
				}
				else {
					groundPoint2.setY(groundPoint2.getY() + groundSpacing);
				}
			} while (groundPoint1.getX() < bounds.getX2() - groundSpacing);
		}

		// Trend
		renderTrendArrow(
			renderer,
			bounds.getX() + lineSizeBig,
			centerY,
			altitudeStepUnits,
			altitudeUnitPixels,
			rc.getAltitudeTrendInterpolator().getValue()
		);

		// Autopilot
		renderAutopilotValueIndicator(
			renderer,
			bounds,
			centerY,
			altitudeStepUnits,
			altitudeUnitPixels,
			rc.getAltitudeInterpolator().getValue(),
			rc.getSettings().autopilot.altitude,
			false
		);

		// Current speed
		renderCurrentValue(
			renderer,
			bounds,
			centerY,
			altitude,
			false
		);
	}

	void PFD::renderVerticalSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		auto centerY = bounds.getYCenter();

		// Background
		renderer->renderFilledRectangle(bounds, &Theme::bg2);

		// Lines
		auto lineColor = &Theme::fg4;

		int32_t y = centerY;
		int32_t lineValue = 0;

		bool isBig;

		auto renderLines = [&lineValue, &isBig, renderer, lineColor, &bounds, &y](int32_t yAdder) {
			while (lineValue <= verticalSpeedStepUnitsLimit) {
				isBig = lineValue % verticalSpeedStepUnitsBig == 0;

				if (isBig) {
					renderer->renderHorizontalLine(
						Point(
							bounds.getX(),
							y
						),
						lineSizeBig,
						lineColor
					);

					renderer->renderString(
						Point(
							bounds.getX() + lineSizeBig + verticalSpeedTextOffset,
							y - Theme::fontSmall.getHeight() / 2
						),
						&Theme::fontSmall,
						lineColor,
						std::to_wstring(lineValue / 1000)
					);
				}
				else {
					renderer->renderHorizontalLine(
						Point(
							bounds.getX(),
							y
						),
						lineSizeSmall,
						lineColor
					);
				}

				y += yAdder;
				lineValue += verticalSpeedStepUnits;
			}
		};

		renderLines(-verticalSpeedStepPixels);

		y = centerY + verticalSpeedStepPixels;
		lineValue = verticalSpeedStepUnits;
		renderLines(verticalSpeedStepPixels);

		// Current value
		renderer->renderLine(
			Point(bounds.getX(), centerY - (int32_t) (rc.getVerticalSpeedInterpolator().getValue() * (float) verticalSpeedStepPixels / (float) verticalSpeedStepUnits)),
			Point(bounds.getX2(), centerY - (int32_t) (rc.getVerticalSpeedInterpolator().getValue() * (float) verticalSpeedStepPixelsRight / (float) verticalSpeedStepUnits)),
			&Theme::green
		);
	}

	void PFD::renderMiniPanel(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, std::wstring_view text, int8_t textXOffset) {
		// Background
		renderer->renderFilledRectangle(bounds, bg);

		// Text
		renderer->renderString(
			Point(
				bounds.getX() + textXOffset + (bounds.getWidth() - textXOffset) / 2 - Theme::fontSmall.getWidth(text) / 2,
				bounds.getY() + miniHeight / 2 - Theme::fontSmall.getHeight() / 2
			),
			&Theme::fontSmall,
			fg,
			text
		);
	}

	void PFD::renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, uint16_t autopilotValue, bool left) {
		renderMiniPanel(
			renderer,
			bounds,
			bg,
			fg,
			autopilotValue > 0 ? std::to_wstring(autopilotValue) : L"----",
			(int8_t) (left ? -autopilotIndicatorTriangleWidth : autopilotIndicatorTriangleWidth)
		);

		if (autopilotValue > 0) {
			renderAutopilotValueIndicator(
				renderer,
				Point(
					left ? bounds.getX2() - autopilotIndicatorTriangleWidth : bounds.getX(),
					bounds.getY()
				),
				left
			);
		}
	}

	void PFD::renderAutopilotSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::blue;

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.speed, true);
	}

	void PFD::renderAutopilotAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::blue;

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.altitude, false);
	}

	void PFD::renderPressure(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::blue;

		std::wstring text;

		switch (rc.getSettings().controls.altimeterMode) {
			case AltimeterMode::QNH:
				text = std::to_wstring((uint16_t) rc.getAltimeterPressure());
				break;

			case AltimeterMode::QNE:
				text = L"STD";
				bg = &Theme::yellow;
				fg = &Theme::bg1;

				break;
		}

		renderMiniPanel(renderer, bounds, bg, fg, text, 0);
	}

	void PFD::onRender(Renderer* renderer, const Bounds& bounds) {
		renderSyntheticVision(renderer, Bounds(
			bounds.getX() + speedWidth,
			bounds.getY(),
			bounds.getWidth() - speedWidth - altitudeWidth - verticalSpeedWidth,
			bounds.getHeight()
		));

		renderSpeed(renderer, Bounds(
			bounds.getX(),
			bounds.getY() + miniHeight,
			speedWidth,
			bounds.getHeight() - miniHeight
		));

		renderAutopilotSpeed(renderer, Bounds(
			bounds.getX(),
			bounds.getY(),
			speedWidth,
			miniHeight
		));

		renderAltitude(renderer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY() + miniHeight,
			altitudeWidth,
			bounds.getHeight() - miniHeight * 2
		));

		renderAutopilotAltitude(renderer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY(),
			altitudeWidth,
			miniHeight
		));

		renderPressure(renderer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY2() + 1 - miniHeight,
			altitudeWidth,
			miniHeight
		));

		renderVerticalSpeed(renderer, Bounds(
			bounds.getX2() + 1 - verticalSpeedWidth,
			bounds.getY(),
			verticalSpeedWidth,
			bounds.getHeight()
		));
	}
}