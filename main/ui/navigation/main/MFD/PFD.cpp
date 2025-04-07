#include "PFD.h"
#include <numbers>

#include "../../../../rc.h"
#include "../../../theme.h"
#include "../../../../units.h"

namespace pizda {
	PFD::PFD() {
		setClipToBounds(true);
	}

	void PFD::onTick() {
		Element::onTick();

		invalidate();
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
			bounds.getHeight() - miniHeight * 2
		));

		renderGroundSpeed(renderer, Bounds(
			bounds.getX(),
			bounds.getY2() + 1 - miniHeight,
			speedWidth,
			miniHeight
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

		if (value < 0)
			value = 0;

		auto uintValue = (uint32_t) value;

		// Assuming 4 is "widest" digit
		const uint8_t maxDigitWidth = currentValueFont->getWidth(L'4');

		int32_t x =
			left
			? bounds.getX2() - currentValueTriangleSize - textOffset
			: bounds.getX() + currentValueTriangleSize + textOffset + maxDigitWidth * getDigitCount(uintValue);

		y = y + currentValueHeight / 2 - currentValueFont->getHeight() / 2;

		float integer;
		const auto fractional = std::modf(value, &integer);
		const int32_t scrolledY = y + (uint8_t) (fractional * (float) currentValueFont->getHeight());

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
					x - currentValueFont->getWidth(text),
					digitY
				),
				currentValueFont,
				&Theme::fg1,
				text
			);
		};

		auto shouldScroll = true;
		uint8_t digit;

		do {
			digit = uintValue % 10;

			if (shouldScroll) {
				renderDigit(scrolledY - currentValueFont->getHeight(), getAdjacentDigit(digit, true));
				renderDigit(scrolledY, digit);
				renderDigit(scrolledY + currentValueFont->getHeight(), getAdjacentDigit(digit, false));
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

	void PFD::renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t stepPixels, float currentValue, uint16_t autopilotValue, bool left) {
		if (autopilotValue == 0)
			return;

		renderAutopilotValueIndicator(
			renderer,
			Point(
				left ? bounds.getX2() + 1 - autopilotIndicatorWidth : bounds.getX(),
				centerY + (int32_t) ((currentValue - (float) autopilotValue) * (float) stepPixels / (float) unitStep) - autopilotIndicatorHeightHalf
			),
			left
		);
	}

	void PFD::renderTrendArrow(Renderer* renderer, int32_t x, int32_t y, uint8_t unitStep, uint16_t stepPixels, float value) {
		const auto length = (int32_t) ((float) stepPixels * value / (float) unitStep);

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

		const float speed = rc.getAirSpeedInterpolator().getValue();

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

		// Not enough lift in any case
		renderBar(
			barX,
			speedBarSize,
			0,
			speedFlapsMin,
			&Theme::red
		);

		// Flaps only
		renderBar(
			barX,
			speedBarSize,
			speedFlapsMin,
			speedSmoothMin,
			&Theme::fg1
		);

		// Flaps (& smooth)
		renderBar(
			barX,
			speedBarSize / 2,
			speedSmoothMin,
			speedFlapsMax,
			&Theme::fg1
		);

		// Smooth (& flaps)
		renderBar(
			barX + speedBarSize / 2,
			speedBarSize / 2,
			speedSmoothMin,
			speedFlapsMax,
			&Theme::greenSpeed
		);

		// Smooth (no flaps)
		renderBar(
			barX,
			speedBarSize,
			speedFlapsMax,
			speedSmoothMax,
			&Theme::greenSpeed
		);

		// Turbulent
		renderBar(
			barX,
			speedBarSize,
			speedTurbulentMin,
			speedTurbulentMax,
			&Theme::yellow
		);

		// Structural
		renderBar(
			barX,
			speedBarSize,
			speedStructuralMin,
			speedStructuralMax,
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
						bounds.getX2() + 1 - speedBarSize - lineSizeBig - lineSizeTextOffset - currentValueFont->getWidth(text),
						y - currentValueFont->getHeight() / 2
					),
					currentValueFont,
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

		// V-speeds
		for (const auto& VSpeed : VSpeeds) {
			const auto& VSpeedBounds = Bounds(
				bounds.getX2() + VSpeedMargin + VSpeedTriangleWidth,
				centerY - (int32_t) (((float) VSpeed.getValue() - speed) * (float) speedStepPixels / (float) speedStepUnits),
				Theme::fontSmall.getWidth(VSpeed.getName()) + VSpeedTextOffset * 2,
				Theme::fontSmall.getHeight() + VSpeedTextOffset * 2
			);

			// Rect
			renderer->renderFilledRectangle(
				VSpeedBounds,
				1,
				&Theme::bg2
			);

			// Triangle
			renderer->renderFilledTriangle(
				VSpeedBounds.getTopLeft(),
				VSpeedBounds.getBottomLeft(),
				Point(VSpeedBounds.getX() - VSpeedTriangleWidth, VSpeedBounds.getYCenter()),
				&Theme::bg2
			);

			// Text
			renderer->renderString(
				Point(VSpeedBounds.getX() + VSpeedTextOffset, VSpeedBounds.getY() + VSpeedTextOffset),
				&Theme::fontSmall,
				&Theme::green,
				VSpeed.getName()
			);
		}

		// Autopilot
		renderAutopilotValueIndicator(
			renderer,
			bounds,
			centerY,
			speedStepUnits,
			speedStepPixels,
			speed,
			rc.getSettings().autopilot.speedKt,
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
		const Point& horizonRight,
		const Vector2F& horizonVec,
		const Vector2F& horizonVecNorm,
		const Vector2F& horizonVecPerp,
		const Vector2F& horizonVecCenter
	) {
		const auto viewport = renderer->pushViewport(bounds);

		const float lineAngleStepRad = toRadians(pitchOverlayAngleStepDeg);
		const float linesInTotal = std::floorf(std::numbers::pi_v<float> / 2.f / lineAngleStepRad);
		const float linePixelStep = unfoldedFOVHeight / 2.f / linesInTotal;

		Vector2F
			lineCenterVerticalPerp,
			lineVec;

		Point
			lineLeft,
			lineRight;

		const Color* color;

		for (int32_t lineAngleDeg = -90; lineAngleDeg <= 90; lineAngleDeg += pitchOverlayAngleStepDeg) {
			color = lineAngleDeg >= 0 ? pitchOverlayColorGround : pitchOverlayColorSky;
			lineCenterVerticalPerp = horizonVecCenter + horizonVecPerp * ((float) lineAngleDeg / (float) pitchOverlayAngleStepDeg * linePixelStep);

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
						lineRight.getY() - pitchOverlayFont->getHeight() / 2
					),
					pitchOverlayFont,
					color,
					std::to_wstring(abs(lineAngleDeg))
				);
			}
		}

		renderer->popViewport(viewport);
	}

	void PFD::renderTurnCoordinatorOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		float roll
	) {
		const auto& center = Point(
			bounds.getXCenter(),
			bounds.getY() + turnCoordinatorOverlayRollIndicatorRadius
		);

		const auto& renderLine = [renderer, &roll, &center](int8_t angle, bool isBig) {
			const auto vec = Vector2F(0, turnCoordinatorOverlayRollIndicatorRadius).rotate(toRadians(angle) - roll);
			const auto lineFrom = center - (Point) vec;

			renderer->renderLine(
				lineFrom,
				lineFrom + (Point) (vec.normalize() * (isBig ? turnCoordinatorOverlayRollIndicatorLineBigLength : turnCoordinatorOverlayRollIndicatorLineSmallLength)),
				turnCoordinatorOverlayColor
			);
		};

		renderLine(-60, true);
		renderLine(-45, false);
		renderLine(-30, true);
		renderLine(-20, false);
		renderLine(-10, false);

		renderLine(10, false);
		renderLine(20, false);
		renderLine(30, true);
		renderLine(45, false);
		renderLine(60, true);

		// Upper triangle
		renderer->renderFilledTriangle(
			center + (Point) Vector2F(-turnCoordinatorOverlayRollIndicatorTriangleWidth / 2, -turnCoordinatorOverlayRollIndicatorRadius).rotate(-roll),
			center + (Point) Vector2F(turnCoordinatorOverlayRollIndicatorTriangleWidth / 2, -turnCoordinatorOverlayRollIndicatorRadius).rotate(-roll),
			center + (Point) Vector2F(0, -turnCoordinatorOverlayRollIndicatorRadius + turnCoordinatorOverlayRollIndicatorTriangleHeight).rotate(-roll),
			turnCoordinatorOverlayColor
		);

		// Lower triangle
		const int32_t rollTriangleY = bounds.getY() + turnCoordinatorOverlayRollIndicatorTriangleHeight + turnCoordinatorOverlayRollIndicatorTriangleOffset;

		renderer->renderFilledTriangle(
			Point(center.getX(), rollTriangleY),
			Point(center.getX() - turnCoordinatorOverlayRollIndicatorTriangleWidth / 2, rollTriangleY + turnCoordinatorOverlayRollIndicatorTriangleHeight),
			Point(center.getX() + turnCoordinatorOverlayRollIndicatorTriangleWidth / 2, rollTriangleY + turnCoordinatorOverlayRollIndicatorTriangleHeight),
			turnCoordinatorOverlayColor
		);

		// Slip/skid indicator
		renderer->renderFilledRectangle(
			Bounds(
				center.getX() + (int32_t) ((float) turnCoordinatorOverlaySlipAndSkidIndicatorMaxValuePixels * RC::getInstance().getSlipAndSkidInterpolator().getValue()) - turnCoordinatorOverlaySlipAndSkidIndicatorWidth / 2,
				rollTriangleY + turnCoordinatorOverlayRollIndicatorTriangleHeight + turnCoordinatorOverlaySlipAndSkidIndicatorOffset,
				turnCoordinatorOverlaySlipAndSkidIndicatorWidth,
				turnCoordinatorOverlaySlipAndSkidIndicatorHeight
			),
			turnCoordinatorOverlayColor
		);
	}

	void PFD::renderYawOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		float yaw
	) {
		const auto viewport = renderer->pushViewport(bounds);

		const auto centerX = bounds.getXCenter();
		const auto y2 = bounds.getY2();

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
			lineY = y2 - lineLength + 1;

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
						x - yawOverlayFont->getWidth(text) / 2,
						lineY - yawOverlayTextOffset - yawOverlayFont->getHeight()
					),
					yawOverlayFont,
					yawOverlayColor,
					text
				);
			}

			x += yawOverlayAngleStepPixels;
			angle += yawOverlayAngleStepUnits;

			if (angle >= 360)
				angle -= 360;
		}

		// Small triangle representing current heading
		renderer->renderFilledTriangle(
			Point(centerX, y2 - yawOverlayTriangleHeight),
			Point(centerX - yawOverlayTriangleWidth / 2, y2),
			Point(centerX + yawOverlayTriangleWidth / 2, y2),
			yawOverlayColor
		);

		renderer->popViewport(viewport);
	}

	void PFD::renderAircraftSymbolAndFPVOverlay(
		Renderer* renderer,
		const Point& center,
		float unfoldedFOVWidth,
		float unfoldedFOVHeight,
		const Vector2F& horizonVecCenter
	) const {
		auto& rc = RC::getInstance();

		// Aircraft symbol

		// Left
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
				center.getX() + aircraftSymbolCenterOffset + aircraftSymbolThickness,
				center.getY() - aircraftSymbolThickness / 2,
				aircraftSymbolWidth,
				aircraftSymbolThickness
			),
			&Theme::bg1
		);

		// Flight path vector
		if (rc.getGroundSpeed() > speedFlapsMin) {
			auto FPVPosition = Point(
				(int32_t) (horizonVecCenter.getX() + unfoldedFOVWidth * rc.getFlightPathVectorYawInterpolator().getValue() / std::numbers::pi_v<float>),
				(int32_t) (horizonVecCenter.getY() - unfoldedFOVHeight * rc.getFlightPathVectorPitchInterpolator().getValue() / std::numbers::pi_v<float>)
			);

			// Circle
			for (uint8_t i = 0; i < flightPathVectorLineThickness; i++) {
				renderer->renderCircle(
					FPVPosition,
					flightPathVectorRadius - i,
					&Theme::bg1
				);
			}

			// Left line
			renderer->renderFilledRectangle(
				Bounds(
					FPVPosition.getX() - flightPathVectorRadius - flightPathVectorLineLength,
					FPVPosition.getY() - flightPathVectorLineThickness / 2,
					flightPathVectorLineLength,
					flightPathVectorLineThickness
				),
				&Theme::bg1
			);

			// Right line
			renderer->renderFilledRectangle(
				Bounds(
					FPVPosition.getX() + flightPathVectorRadius,
					FPVPosition.getY() - flightPathVectorLineThickness / 2,
					flightPathVectorLineLength,
					flightPathVectorLineThickness
				),
				&Theme::bg1
			);
		}
		// Dot
		else {
			renderer->renderFilledRectangle(
				Bounds(
					center.getX() + aircraftSymbolThickness / 2,
					center.getY() - aircraftSymbolThickness / 2,
					aircraftSymbolThickness,
					aircraftSymbolThickness
				),
				&Theme::bg1
			);
		}
	}

	void PFD::renderSyntheticVision(Renderer* renderer, const Bounds& bounds) const {
		auto& rc = RC::getInstance();

		const auto& center = bounds.getCenter();

		const auto pitch = rc.getPitchInterpolator().getValue();
		const auto roll = rc.getRollInterpolator().getValue();
		const auto yaw = rc.getYawInterpolator().getValue();

		// FOV deg - Screen px
		// 180 deg - x px
		// x = 180 * screen / FOV
		const auto unfoldedFOVWidth = std::numbers::pi_v<float> * (float) bounds.getWidth() / _horizontalFOV;
		const auto unfoldedFOVHeight = std::numbers::pi_v<float> * (float) bounds.getHeight() / _verticalFOV;

		const auto& horizonRollRotated = (Point) Vector2F(unfoldedFOVWidth / 2, 0).rotate(-roll);
		const auto horizonPitchOffset = pitch / std::numbers::pi_v<float> * unfoldedFOVHeight;

		const auto& horizonLeft = Point(
			center.getX() - horizonRollRotated.getX(),
			center.getY() + horizonPitchOffset - horizonRollRotated.getY()
		);

		const auto& horizonRight = Point(
			center.getX() + horizonRollRotated.getX(),
			center.getY() + horizonPitchOffset + horizonRollRotated.getY()
		);

		const auto& horizonVec = (Vector2F) (horizonRight - horizonLeft);
		const auto& horizonVecNorm = horizonVec.normalize();
		const auto& horizonVecPerp = horizonVecNorm.perpendicular();
		const auto& horizonVecCenter = (Vector2F) horizonLeft + horizonVec / 2.0f;

		// Background
		renderSyntheticVisionBackground(
			renderer,
			bounds,
			horizonLeft,
			horizonRight
		);

		// Roll overlay
		renderTurnCoordinatorOverlay(
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
			unfoldedFOVHeight,
			horizonLeft,
			horizonRight,
			horizonVec,
			horizonVecNorm,
			horizonVecPerp,
			horizonVecCenter
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

		// Wind
		if (rc.getGroundSpeed() > windVisibilityGroundSpeed) {
			renderWind(
				renderer,
				Point(
					bounds.getX() + 6,
					bounds.getY2() - 18
				)
			);
		}

		// FPV
		renderAircraftSymbolAndFPVOverlay(
			renderer,
			center,
			unfoldedFOVWidth,
			unfoldedFOVHeight,
			horizonVecCenter
		);
	}

	void PFD::renderAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto centerY = bounds.getYCenter();
		auto x = bounds.getX();

		renderer->renderFilledRectangle(bounds, &Theme::bg1);

		const float altitude = rc.getAltitudeInterpolator().getValue();
		float snapped = altitude / (float) altitudeStepUnits;
		float snappedInteger = std::floorf(snapped);
		float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) altitudeStepPixels);
		auto yFullLines = (int32_t) std::ceilf((float) y / (float) altitudeStepPixels);
		y = y - yFullLines * altitudeStepPixels;

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
					Point(x + lineSizeBig + lineSizeTextOffset, y - currentValueFont->getHeight() / 2),
					currentValueFont,
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
			y += altitudeStepPixels;
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
			altitudeStepPixels,
			rc.getAltitudeTrendInterpolator().getValue()
		);

		// Minimums
		if (rc.getSettings().controls.minimumAltitudeEnabled) {
			const int32_t delta = altitude - (float) rc.getSettings().controls.minimumAltitudeFt;

			y = centerY + (int32_t) (delta * (float) altitudeStepPixels / (float) altitudeStepUnits);

			const auto& linePosition = Point(bounds.getX() - altitudeMinimumHorizontalOffset + altitudeMinimumTriangleWidth, y);

			const Color* color;

			if (std::abs(delta) <= altitudeMinimumSafeUnitDelta) {
				color = &Theme::fg1;
			}
			else if (delta > 0) {
				color = &Theme::green;
			}
			else{
				color = &Theme::yellow;
			}

			// Line
			renderer->renderHorizontalLine(
				linePosition,
				bounds.getWidth() + altitudeMinimumHorizontalOffset - altitudeMinimumTriangleWidth,
				color
			);

			// Triangle
			renderer->renderFilledTriangle(
				linePosition,
				Point(linePosition.getX() - altitudeMinimumTriangleWidth, linePosition.getY() - altitudeMinimumTriangleHeight),
				Point(linePosition.getX() - altitudeMinimumTriangleWidth, linePosition.getY() + altitudeMinimumTriangleHeight),
				color
			);
		}

		// Autopilot
		renderAutopilotValueIndicator(
			renderer,
			bounds,
			centerY,
			altitudeStepUnits,
			altitudeStepPixels,
			altitude,
			rc.getSettings().autopilot.altitudeFt,
			false
		);

		// Current value
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
							y - verticalSpeedFont->getHeight() / 2
						),
						verticalSpeedFont,
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
				bounds.getX() + textXOffset + (bounds.getWidth() - textXOffset) / 2 - miniFont->getWidth(text) / 2,
				bounds.getY() + miniHeight / 2 - miniFont->getHeight() / 2
			),
			miniFont,
			fg,
			text
		);
	}

	void PFD::renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, uint16_t autopilotValue, bool autopilotValueEnabled, bool left) {
		renderMiniPanel(
			renderer,
			bounds,
			bg,
			fg,
			autopilotValueEnabled ? std::to_wstring(autopilotValue) : L"----",
			(int8_t) (left ? -autopilotIndicatorTriangleWidth : autopilotIndicatorTriangleWidth)
		);

		if (autopilotValueEnabled) {
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

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.speedKt, rc.getSettings().autopilot.speedKt > 0, true);
	}

	void PFD::renderAutopilotAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::blue;

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.altitudeFt, rc.getSettings().autopilot.altitudeFt > 0, false);
	}

	void PFD::renderPressure(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::yellow;

		std::wstring text;

		if (rc.getSettings().controls.referencePressureSTD) {
			text = L"STD";
			bg = &Theme::yellow;
			fg = &Theme::bg1;
		}
		else {
			text = std::to_wstring((uint32_t) convertPressure(rc.getSettings().controls.referencePressurePa, PressureUnit::pascal, PressureUnit::hectopascal));
		}

		renderMiniPanel(renderer, bounds, bg, fg, text, 0);
	}

	void PFD::renderGroundSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		renderMiniPanel(renderer, bounds, &Theme::bg2, &Theme::purple, std::to_wstring((uint16_t) rc.getGroundSpeed()), 0);
	}

	void PFD::renderWind(Renderer* renderer, const Point& bottomLeft) const {
		auto& rc = RC::getInstance();

		const uint8_t textOffset = 4;
		const auto text = std::to_wstring((uint16_t) rc.getWindSpeed());
		const uint8_t arrowSize = 16;

		const auto arrowVec = Vector2F(0, arrowSize).rotate(rc.getWindDirectionInterpolator().getValue() + std::numbers::pi_v<float> - rc.getYawInterpolator().getValue());
		const auto arrowVecNorm = arrowVec.normalize();
		const auto arrowVecPerp = arrowVecNorm.perpendicular();

		const auto arrowCenter = Vector2F (
			bottomLeft.getX() + arrowSize / 2,
			bottomLeft.getY() - Theme::fontSmall.getHeight() - textOffset - arrowSize / 2
		);

		const auto arrowToVec = (Vector2F) arrowCenter - arrowVec / 2.f;

		renderer->renderLine(
			(Point) (arrowCenter + arrowVec / 2.f),
			(Point) arrowToVec,
			&Theme::ground2
		);

		const uint8_t triangleWidth = 2;
		const uint8_t triangleHeight = 3;

		renderer->renderFilledTriangle(
			(Point) arrowToVec,
			(Point) (arrowToVec + arrowVecNorm * triangleHeight - arrowVecPerp * triangleWidth),
			(Point) (arrowToVec + arrowVecNorm * triangleHeight + arrowVecPerp * triangleWidth),
			&Theme::ground2
		);

		renderer->renderString(
			Point(
				arrowCenter.getX() - Theme::fontSmall.getWidth(text) / 2,
				bottomLeft.getY() - Theme::fontSmall.getHeight()
			),
			&Theme::fontSmall,
			&Theme::ground2,
			text
		);
	}
}