#include "pfd.h"
#include "../../rc_application.h"
#include <sstream>

namespace pizdanc {
	// No errors here, just linter's being pussy
	const Color* PFD::rollOverlayColor = &Theme::sky2;
	const Color* PFD::pitchOverlayColorSky = &Theme::sky2;
	const Color* PFD::pitchOverlayColorGround = &Theme::ground2;
	const Color* PFD::yawOverlayColor = &Theme::ground2;

	PFD::PFD() {
		setClipToBounds(true);
	}

	void PFD::renderCurrentValue(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, float value, bool left) {
		int32_t y = centerY - currentValueHeight / 2;

		// Triangle
		screenBuffer->renderFilledTriangle(
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

		screenBuffer->renderFilledRectangle(rectangleBounds, &Theme::bg2);

		// Text
		const uint8_t textOffset = 2;

		const auto oldViewport = screenBuffer->getViewport();
		screenBuffer->setViewport(rectangleBounds);

		auto uintValue = (uint32_t) value;

		// Assuming 8 is "widest" digit
		const uint8_t maxDigitWidth = Theme::fontNormal.getCharWidth(L'8');
		const uint16_t maxTextWidth = maxDigitWidth * getDigitCount(uintValue);

		int32_t x =
			left
			? bounds.getX2() - currentValueTriangleSize - textOffset
			: bounds.getX() + currentValueTriangleSize + textOffset + maxTextWidth;

		y = y + currentValueHeight / 2 - Theme::fontNormal.getHeight() / 2;

		float integer;
		const auto fractional = modf(value, &integer);
		const int32_t scrolledY = y + (uint8_t) (fractional * (float) Theme::fontNormal.getHeight());

		auto getNextDigit = [&](uint8_t digit, bool plus) {
			return
				plus
				? (digit < 9 ? digit + 1 : 0)
				: (digit > 1 ? digit - 1 : 9);
		};

		auto renderDigit = [&](int32_t digitY, uint8_t digit) {
			static std::wstringstream stream;
			stream.str(std::wstring());
			stream.clear();

			stream << digit;

			const auto& text = stream.str();

			screenBuffer->renderChar(
				Point(
					x - Theme::fontNormal.getCharWidth(text[0]),
					digitY
				),
				&Theme::fontNormal,
				&Theme::fg1,
				text[0]
			);
		};

		auto shouldScroll = true;
		uint8_t digit;

		do {
			digit = uintValue % 10;

			if (shouldScroll) {
				renderDigit(scrolledY - Theme::fontNormal.getHeight(), getNextDigit(digit, true));
				renderDigit(scrolledY, digit);
				renderDigit(scrolledY + Theme::fontNormal.getHeight(), getNextDigit(digit, false));
			}
			else {
				renderDigit(y, digit);
			}

			x -= maxDigitWidth;

			shouldScroll = digit == 9;
			uintValue /= 10;
		} while (uintValue > 0);

		screenBuffer->setViewport(oldViewport);
	}

	void PFD::renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Point& point, bool left) {
		// Upper
		screenBuffer->renderFilledRectangle(
			Bounds(
				point.getX(),
				point.getY(),
				autopilotIndicatorWidth,
				autopilotIndicatorTriangleVerticalMargin
			),
			&Theme::blue
		);

		// Lower
		screenBuffer->renderFilledRectangle(
			Bounds(
				point.getX(),
				point.getY() + autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin,
				autopilotIndicatorWidth,
				autopilotIndicatorTriangleVerticalMargin
			),
			&Theme::blue
		);

		// Rect
		screenBuffer->renderFilledRectangle(
			Bounds(
				left ? point.getX() + autopilotIndicatorTriangleWidth : point.getX(),
				point.getY() + autopilotIndicatorTriangleVerticalMargin,
				autopilotIndicatorRectangleWidth,
				autopilotIndicatorTriangleHeight
			),
			&Theme::blue
		);

		// Upper triangle
		screenBuffer->renderFilledTriangle(
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
		screenBuffer->renderFilledTriangle(
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

	void PFD::renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, float autopilotValue, bool left) {
		if (autopilotValue <= 0)
			return;

		renderAutopilotValueIndicator(
			screenBuffer,
			Point(
				left ? bounds.getX2() + 1 - autopilotIndicatorWidth : bounds.getX(),
				centerY + (int32_t) ((currentValue - autopilotValue) * (float) unitPixels / (float) unitStep) - autopilotIndicatorHeightHalf
			),
			left
		);
	}

	void PFD::renderTrendArrow(ScreenBuffer* screenBuffer, int32_t x, int32_t y, uint8_t unitStep, uint16_t unitPixels, float value) {
		auto length = (int32_t) ((float) unitPixels * value / (float) unitStep);

		if (abs(length) < 10)
			return;

		const uint8_t arrowSize = 3;

		auto yArrow = y - length - arrowSize;
		auto yMin = min(y, yArrow);
		auto yMax = max(y, yArrow);

		screenBuffer->renderVerticalLine(
			Point(x, yMin),
			yMax - yMin,
			&Theme::purple
		);

		screenBuffer->renderFilledTriangle(
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

	void PFD::renderSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) {
		auto& app = RCApplication::getInstance();

		auto centerY = bounds.getYCenter();

		screenBuffer->renderFilledRectangle(bounds, &Theme::bg1);

		float speed = app.getSpeedInterpolator().getValue();

		// Bars
		const auto barX = bounds.getX2() + 1 - speedBarSize;

		const auto renderBar = [&](int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color *color) {
			int32_t fromY = centerY + (int32_t) ceil(speed * (float) speedStepPixels - (float) fromSpeed * (float) speedStepPixels);
			int32_t height = (toSpeed - fromSpeed) * speedStepPixels;

			screenBuffer->renderFilledRectangle(
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
		float snappedInteger = floor(snapped);
		float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) speedStepPixels);
		int32_t altitudeYFullLines = ceil((float) y / (float) speedStepPixels);
		y = y - altitudeYFullLines * speedStepPixels;

		int32_t lineValue = (int32_t) (snappedInteger + 1) * speedStepUnits + altitudeYFullLines * speedStepUnits;

		static std::wstringstream stream;
		Size textSize;
		bool isBig;
		const Color *lineColor = &Theme::fg3;

		do {
			isBig = lineValue % speedStepUnitsBig == 0;

			if (isBig) {
				// Line
				screenBuffer->renderHorizontalLine(
					Point(
						bounds.getX2() + 1 - speedBarSize - lineSizeBig,
						y
					),
					lineSizeBig,
					lineColor
				);

				// Text
				stream.str(std::wstring());
				stream.clear();

				stream << lineValue;

				const auto& text = stream.str();

				textSize = Theme::fontNormal.getSize(text);

				screenBuffer->renderText(
					Point(
						bounds.getX2() + 1 - speedBarSize - lineSizeBig - 5 - textSize.getWidth(),
						y - textSize.getHeight() / 2
					),
					&Theme::fontNormal,
					lineColor,
					text
				);
			}
			else {
				// Line
				screenBuffer->renderHorizontalLine(
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
			screenBuffer,
			bounds.getX2() - speedBarSize - lineSizeBig,
			centerY,
			speedStepUnits,
			speedStepPixels,
			app.getSpeedTrendInterpolator().getValue()
		);

		// Autopilot
		renderAutopilotValueIndicator(
			screenBuffer,
			bounds,
			centerY,
			speedStepUnits,
			speedStepPixels,
			speed,
			app.getLocalData().getAutopilotSpeed(),
			true
		);

		// Current speed
		renderCurrentValue(
			screenBuffer,
			bounds,
			centerY,
			speed,
			true
		);
	}

	void PFD::renderAircraftSymbol(ScreenBuffer* screenBuffer, const Point& center) {
//		const uint8_t sideWidth = 16;
//		const uint8_t radius = 6;
//
//		screenBuffer->renderCircle(
//			center,
//			radius,
//			&Theme::fg1
//		);
//
//		screenBuffer->renderHorizontalLine(
//			Point(
//				center.getX() - radius - sideWidth,
//				center.getY()
//			),
//			sideWidth,
//			&Theme::fg1
//		);
//
//		screenBuffer->renderHorizontalLine(
//			Point(
//				center.getX() + radius,
//				center.getY()
//			),
//			sideWidth,
//			&Theme::fg1
//		);

		const uint8_t aircraftSymbolWidth = 30;
		const uint8_t aircraftSymbolThickness = 2;
		const uint8_t aircraftSymbolHeight = 6;
		const uint8_t aircraftSymbolCenterOffset = 16;

		// Left
		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() - aircraftSymbolCenterOffset - aircraftSymbolThickness,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolThickness,
				aircraftSymbolHeight
			),
			&Theme::bg1
		);

		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() - aircraftSymbolCenterOffset - aircraftSymbolThickness - aircraftSymbolWidth,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolWidth,
				aircraftSymbolThickness
			),
			&Theme::bg1
		);

		// Right
		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() + aircraftSymbolCenterOffset,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolThickness,
				aircraftSymbolHeight
			),
			&Theme::bg1
		);

		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() + aircraftSymbolCenterOffset + aircraftSymbolThickness,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolWidth,
				aircraftSymbolThickness
			),
			&Theme::bg1
		);

		// Dot
		screenBuffer->renderFilledRectangle(
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
		ScreenBuffer* screenBuffer,
		const Bounds& bounds,
		const Point& horizonLeft,
		const Point& horizonRight
	) {
		// Sky
		screenBuffer->renderFilledRectangle(bounds, &Theme::sky);

		// Ground
		const auto groundMaxY = max(horizonLeft.getY(), horizonRight.getY());

		// Triangle
		screenBuffer->renderFilledTriangle(
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
			screenBuffer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					groundMaxY,
					bounds.getWidth(),
					bounds.getHeight() - groundMaxY
				),
				&Theme::ground
			);
		}
			// Left
		else if (horizonLeft.getY() < bounds.getY() && horizonLeft.getX() > bounds.getX()) {
			screenBuffer->renderFilledRectangle(
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
			screenBuffer->renderFilledRectangle(
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
		ScreenBuffer* screenBuffer,
		const Bounds& bounds,
		float unfoldedFOVHeight,
		const Point& horizonLeft,
		const Point& horizonRight
	) {
		screenBuffer->setViewport(bounds);

		const float lineAngleStepRad = radians(pitchOverlayAngleStep);
		const float linesInTotal = floor(((float) HALF_PI) / lineAngleStepRad);
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

		std::wstringstream stream;
		Size textSize;
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

			screenBuffer->renderLine(
				lineLeft,
				lineRight,
				color
			);

			if (lineAngleDeg != 0 && lineAngleDeg % 10 == 0) {
				stream.str(std::wstring());
				stream.clear();

				stream << abs(lineAngleDeg);

				const auto& text = stream.str();

				textSize = Theme::fontSmall.getSize(text);

				screenBuffer->renderText(
					Point(
						lineRight.getX() + pitchOverlayTextOffset,
						lineRight.getY() - textSize.getHeight() / 2
					),
					&Theme::fontSmall,
					color,
					text
				);
			}
		}
	}

	void PFD::renderRollOverlay(
		ScreenBuffer* screenBuffer,
		const Bounds& bounds,
		float roll
	) {
		screenBuffer->setViewport(bounds);

		const float radius = (float) bounds.getWidth() * 1.0f;

		const auto center = Point(
			bounds.getXCenter(),
			bounds.getY() + ((uint16_t) radius) + 2
		);

		float closestInteger;
		float closestFractional = modff(degrees(roll) / (float) rollOverlayAngleStep, &closestInteger);
		closestInteger *= (float) rollOverlayAngleStep;

		Size textSize;

		for (int8_t i = -rollOverlayLinesCount; i <= rollOverlayLinesCount; i++) {
			const float angle = (float) i * (float) rollOverlayAngleStep;

			const auto angleDisplay = (int16_t) round(closestInteger + angle);
			const auto isBig = angleDisplay % 10 == 0;
			const auto& vec = Vector2F(0, -radius).rotate((float) radians(angle - closestFractional * rollOverlayAngleStep));
			const auto& lineTo = center + (Point) vec;
			const auto& lineFrom = lineTo + (Point) (vec.normalize() * (isBig ? rollOverlayLineBigLength : rollOverlayLineSmallLength));

			// Line
			screenBuffer->renderLine(
				lineTo,
				lineFrom,
				rollOverlayColor
			);

			// Text
			if (isBig) {
				static std::wstringstream stream;
				stream.str(std::wstring());
				stream.clear();

				stream << angleDisplay;

				const auto& text = stream.str();

				textSize = Theme::fontSmall.getSize(text);

				screenBuffer->renderText(
					Point(
						lineFrom.getX() - textSize.getWidth() / 2,
						lineFrom.getY() + rollOverlayTextOffset
					),
					&Theme::fontSmall,
					rollOverlayColor,
					text
				);
			}
		}

		// Small triangle representing current roll
		screenBuffer->renderFilledTriangle(
			Point(center.getX() - rollOverlayTriangleSize, bounds.getY()),
			Point(center.getX() + rollOverlayTriangleSize, bounds.getY()),
			Point(center.getX(), bounds.getY() + rollOverlayTriangleSize),
			rollOverlayColor
		);
	}

	void PFD::renderYawOverlay(
		ScreenBuffer* screenBuffer,
		const Bounds& bounds,
		float yaw
	) {
		screenBuffer->setViewport(bounds);

		const auto centerX = bounds.getXCenter();

		float closestInteger;
		float closestFractional = modff(degrees(yaw) / yawOverlayAngleStepUnits, &closestInteger);
		closestInteger *= yawOverlayAngleStepUnits;

		const uint8_t fullCount = (uint8_t) ceil((float) (centerX - bounds.getX()) / yawOverlayAngleStepPixels) + 1;
		int32_t x = centerX - fullCount * yawOverlayAngleStepPixels - (int32_t) (closestFractional * (float) yawOverlayAngleStepPixels);
		auto angle = (int16_t) (closestInteger - (float) (fullCount * yawOverlayAngleStepUnits));

		if (angle < 0)
			angle += 360;

		bool isBig;
		uint8_t lineLength;

		std::wstringstream stream;
		Size textSize;
		int32_t lineY;

		while (x <= bounds.getX2()) {
			isBig = angle % 10 == 0;
			lineLength = isBig ? yawOverlayLineBigLength : yawOverlayLineSmallLength;

			// Line
			lineY = bounds.getY2() - lineLength + 1;

			screenBuffer->renderVerticalLine(
				Point(
					x,
					lineY
				),
				lineLength,
				yawOverlayColor
			);

			// Text
			if (isBig) {
				stream.str(std::wstring());
				stream.clear();

				switch (angle) {
					case 0:
						stream << U'N';
						break;

					case 90:
						stream << U'E';
						break;

					case 180:
						stream << U'S';
						break;

					case 270:
						stream << U'W';
						break;

					default:
						stream << angle;
						break;
				}

				const auto& text = stream.str();

				textSize = Theme::fontSmall.getSize(text);

				screenBuffer->renderText(
					Point(
						x - textSize.getWidth() / 2,
						lineY - yawOverlayTextOffset - textSize.getHeight()
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
		screenBuffer->renderFilledTriangle(
			Point(centerX - yawOverlayTriangleSize, bounds.getY2()),
			Point(centerX + yawOverlayTriangleSize, bounds.getY2()),
			Point(centerX, bounds.getY2() - yawOverlayTriangleSize),
			yawOverlayColor
		);
	}

	void PFD::renderSyntheticVision(ScreenBuffer* screenBuffer, const Bounds& bounds) const {
		auto& app = RCApplication::getInstance();

		const auto& center = bounds.getCenter();

		const auto pitch = app.getPitchInterpolator().getValue();
		const auto roll = app.getRollInterpolator().getValue();
		const auto yaw = app.getYawInterpolator().getValue();

		// value = [180 deg of unfolded full range view] / [FOV deg of camera viewport] * [viewport size in pixels]
		const float unfoldedFovWidth = PI / _horizontalFov / 2 * bounds.getWidth();
		const float unfoldedFovHeight = PI / _verticalFov / 2 * bounds.getHeight();

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
			screenBuffer,
			bounds,
			horizonLeft,
			horizonRight
		);

		const auto oldViewport = screenBuffer->getViewport();

		// Roll overlay
		renderRollOverlay(
			screenBuffer,
			Bounds(
				bounds.getX(),
				bounds.getY(),
				bounds.getWidth(),
				rollOverlayHeight
			),
			roll
		);

		// Pitch overlay
		renderPitchOverlay(
			screenBuffer,
			Bounds(
				bounds.getX(),
				bounds.getY() + rollOverlayHeight,
				bounds.getWidth(),
				bounds.getHeight() - rollOverlayHeight - yawOverlayHeight
			),
			unfoldedFovHeight,
			horizonLeft,
			horizonRight
		);

		// Yaw overlay
		renderYawOverlay(
			screenBuffer,
			Bounds(
				bounds.getX(),
				bounds.getY2() - yawOverlayHeight + 1,
				bounds.getWidth(),
				yawOverlayHeight
			),
			yaw
		);

		screenBuffer->setViewport(oldViewport);

		// Bird
		renderAircraftSymbol(screenBuffer, center);

//		// Temp blyad radio
//
//		swprintf(text, 255, L"RSSI: %.2f dBm", app.getTransceiver().getRssi());
//		screenBuffer->renderText(
//			Point(bounds.getX() + 10, bounds.getY() + 10),
//			&Theme::font,
//			&Theme::fg1,
//			text
//		);
//
//		swprintf(text, 255, L"SNR: %.2f dB", app.getTransceiver().getSnr());
//		screenBuffer->renderText(
//			Point(bounds.getX() + 10, bounds.getY() + 20),
//			&Theme::font,
//			&Theme::fg1,
//			text
//		);
	}

	void PFD::renderAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) {
		auto& app = RCApplication::getInstance();

		auto centerY = bounds.getYCenter();
		auto x = bounds.getX();

		screenBuffer->renderFilledRectangle(bounds, &Theme::bg1);

		float altitude = app.getAltitudeInterpolator().getValue();
		float snapped = altitude / (float) altitudeStepUnits;
		float snappedInteger = floor(snapped);
		float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) altitudeUnitPixels);
		auto yFullLines = (int32_t) ceil((float) y / (float) altitudeUnitPixels);
		y = y - yFullLines * altitudeUnitPixels;

		int32_t lineValue = (int32_t) (snappedInteger + 1) * altitudeStepUnits + yFullLines * altitudeStepUnits;

		std::wstringstream stream;
		Size textSize;
		bool isBig;

		const Color *lineColor = &Theme::fg3;

		do {
			isBig = lineValue % altitudeStepUnitsBig == 0;

			if (isBig) {
				screenBuffer->renderHorizontalLine(
					Point(x, y),
					lineSizeBig,
					lineColor
				);

				//Text
				stream.str(std::wstring());
				stream.clear();
				stream << lineValue;
				const auto& text = stream.str();

				textSize = Theme::fontNormal.getSize(text);

				screenBuffer->renderText(
					Point(x + lineSizeBig + 5, y - textSize.getHeight() / 2),
					&Theme::fontNormal,
					lineColor,
					text
				);
			} else {
				screenBuffer->renderHorizontalLine(
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
				screenBuffer->renderLine(
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
			screenBuffer,
			bounds.getX() + lineSizeBig,
			centerY,
			altitudeStepUnits,
			altitudeUnitPixels,
			app.getAltitudeTrendInterpolator().getValue()
		);

		// Autopilot
		renderAutopilotValueIndicator(
			screenBuffer,
			bounds,
			centerY,
			altitudeStepUnits,
			altitudeUnitPixels,
			app.getAltitudeInterpolator().getValue(),
			app.getLocalData().getAutopilotAltitude(),
			false
		);

		// Current speed
		renderCurrentValue(
			screenBuffer,
			bounds,
			centerY,
			altitude,
			false
		);
	}

	void PFD::renderVerticalSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) {
		auto& app = RCApplication::getInstance();
		auto centerY = bounds.getYCenter();

		// Background
		screenBuffer->renderFilledRectangle(bounds, &Theme::bg2);

		// Lines
		auto lineColor = &Theme::fg4;

		int32_t y = centerY;
		int32_t lineValue = 0;

		Size textSize;
		bool isBig;
		static std::wstringstream stream;

		auto renderLines = [&](int32_t yAdder) {
			while (lineValue <= verticalSpeedStepUnitsLimit) {
				isBig = lineValue % verticalSpeedStepUnitsBig == 0;

				if (isBig) {
					screenBuffer->renderHorizontalLine(
						Point(
							bounds.getX(),
							y
						),
						lineSizeBig,
						lineColor
					);

					stream.str(std::wstring());
					stream.clear();
					stream << lineValue / 100;
					const auto& text = stream.str();

					textSize = Theme::fontNormal.getSize(text);

					screenBuffer->renderText(
						Point(
							bounds.getX() + lineSizeBig + 4,
							y - textSize.getHeight() / 2
						),
						&Theme::fontNormal,
						lineColor,
						text
					);
				}
				else {
					screenBuffer->renderHorizontalLine(
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
		screenBuffer->renderLine(
			Point(bounds.getX(), centerY - (int32_t) (app.getVerticalSpeedInterpolator().getValue() * (float) verticalSpeedStepPixels / (float) verticalSpeedStepUnits)),
			Point(bounds.getX2(), centerY - (int32_t) (app.getVerticalSpeedInterpolator().getValue() * (float) verticalSpeedStepPixelsRight / (float) verticalSpeedStepUnits)),
			&Theme::green
		);
	}

	void PFD::renderMiniPanel(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color *bg, const Color *fg, const std::wstring_view& text, int8_t textXOffset) {
		// Background
		screenBuffer->renderFilledRectangle(bounds, bg);

		// Text
		auto textSize = Theme::fontSmall.getSize(text);

		screenBuffer->renderText(
			Point(
				bounds.getX() + textXOffset + (bounds.getWidth() - textXOffset) / 2 - textSize.getWidth() / 2,
				bounds.getY() + miniHeight / 2 - textSize.getHeight() / 2
			),
			&Theme::fontSmall,
			fg,
			text
		);
	}

	void PFD::renderMiniPanelWithAutopilotValue(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color *bg, const Color *fg, float autopilotValue, bool left) {
		static std::wstringstream stream;
		stream.str(std::wstring());
		stream.clear();

		if (autopilotValue > 0) {
			stream << (int32_t) autopilotValue;
		}
		else {
			stream << L"----";
		}

		const auto& text = stream.str();
		
		renderMiniPanel(
			screenBuffer,
			bounds,
			bg,
			fg,
			text,
			(int8_t) (left ? -autopilotIndicatorTriangleWidth : autopilotIndicatorTriangleWidth)
		);

		if (autopilotValue > 0) {
			renderAutopilotValueIndicator(
				screenBuffer,
				Point(
					left ? bounds.getX2() - autopilotIndicatorTriangleWidth : bounds.getX(),
					bounds.getY()
				),
				left
			);
		}
	}

	void PFD::renderAutopilotSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) {
		auto& app = RCApplication::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::blue;

		renderMiniPanelWithAutopilotValue(screenBuffer, bounds, bg, fg, app.getLocalData().getAutopilotSpeed(), true);
	}

	void PFD::renderAutopilotAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) {
		auto& app = RCApplication::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::blue;

		renderMiniPanelWithAutopilotValue(screenBuffer, bounds, bg, fg, app.getLocalData().getAutopilotAltitude(), false);
	}

	void PFD::renderPressure(ScreenBuffer* screenBuffer, const Bounds& bounds) {
		auto& app = RCApplication::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::blue;

		static std::wstringstream stream;
		stream.str(std::wstring());
		stream.clear();

		switch (app.getLocalData().getAltimeterMode()) {
			case AltimeterMode::QNH:
				stream << (uint16_t) app.getLocalData().getAltimeterPressure();
				break;

			case AltimeterMode::QNE:
				stream << L"STD";
				bg = &Theme::yellow;
				fg = &Theme::bg1;

				break;
		}

		const auto& text = stream.str();

		renderMiniPanel(screenBuffer, bounds, bg, fg, text, 0);
	}

	void PFD::onRender(ScreenBuffer* screenBuffer) {
		auto& bounds = getBounds();

		renderSyntheticVision(screenBuffer, Bounds(
			bounds.getX() + speedWidth,
			bounds.getY(),
			bounds.getWidth() - speedWidth - altitudeWidth - verticalSpeedWidth,
			bounds.getHeight()
		));

		renderSpeed(screenBuffer, Bounds(
			bounds.getX(),
			bounds.getY() + miniHeight,
			speedWidth,
			bounds.getHeight() - miniHeight
		));

		renderAutopilotSpeed(screenBuffer, Bounds(
			bounds.getX(),
			bounds.getY(),
			speedWidth,
			miniHeight
		));

		renderAltitude(screenBuffer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY() + miniHeight,
			altitudeWidth,
			bounds.getHeight() - miniHeight * 2
		));

		renderAutopilotAltitude(screenBuffer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY(),
			altitudeWidth,
			miniHeight
		));

		renderPressure(screenBuffer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY2() + 1 - miniHeight,
			altitudeWidth,
			miniHeight
		));

		renderVerticalSpeed(screenBuffer, Bounds(
			bounds.getX2() + 1 - verticalSpeedWidth,
			bounds.getY(),
			verticalSpeedWidth,
			bounds.getHeight()
		));
	}
}