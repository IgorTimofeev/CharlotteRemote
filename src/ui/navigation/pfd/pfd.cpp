#include "pfd.h"
#include "../../rc_application.h"

namespace pizdanc {
	PFD::PFD() {
		setClipToBounds(true);
	}

	void PFD::renderCurrentValue(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, float currentValue, bool left) const {
		wchar_t text[8];
		swprintf(text, 8, L"%.0f", currentValue);
		auto textSize = Theme::font.getSize(text);

		auto y = centerY - currentValueHeight / 2;

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
			&Theme::bg3
		);

		// Rect
		screenBuffer->renderFilledRectangle(
			Bounds(
				left ? bounds.getX() : bounds.getX() + currentValueTriangleSize,
				y,
				bounds.getWidth() - currentValueTriangleSize,
				currentValueHeight
			),
			&Theme::bg3
		);

		// Text
		const uint8_t textOffset = 2;

		screenBuffer->renderText(
			Point(
				left ? bounds.getX2() - currentValueTriangleSize - textOffset - textSize.getWidth() : bounds.getX() + currentValueTriangleSize + textOffset,
				y + currentValueHeight / 2 - textSize.getHeight() / 2
			),
			&Theme::font,
			&Theme::fg1,
			text
		);
	}

	void PFD::renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Point& point, bool left) const {
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

	void PFD::renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, float autopilotValue, bool left) const {
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

	void PFD::renderSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) const {
		auto &app = RCApplication::getInstance();

		auto centerY = bounds.getYCenter();

		screenBuffer->renderFilledRectangle(
			bounds,
			&Theme::bg2
		);

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

		wchar_t text[12];
		Size textSize;
		bool isBig;
		const Color *lineColor = &Theme::fg3;

		do {
			isBig = lineValue % speedStepUnitsBig == 0;

			if (isBig) {
				// Line
				screenBuffer->renderHorizontalLine(
					Point(bounds.getWidth() - speedBarSize - lineSizeBig, y),
					lineSizeBig,
					lineColor
				);

				//Text
				swprintf(text, 12, L"%d", lineValue);
				textSize = Theme::font.getSize(text);

				screenBuffer->renderText(
					Point(bounds.getWidth() - speedBarSize - lineSizeBig - 5 - textSize.getWidth(), y - textSize.getHeight() / 2),
					&Theme::font,
					lineColor,
					text
				);
			} else {
				// Line
				screenBuffer->renderHorizontalLine(
					Point(bounds.getWidth() - speedBarSize - lineSizeSmall, y),
					lineSizeSmall,
					lineColor
				);
			}

			lineValue -= speedStepUnits;
			y += speedStepPixels;
		} while (y < bounds.getHeight() && lineValue >= 0);

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

	void PFD::renderSyntheticVisionBird(ScreenBuffer* screenBuffer, const Point& center) {
		const uint8_t birdWidth = 38;
		const uint8_t birdThickness = 2;
		const uint8_t birdHeight = 8;
		const uint8_t birdCenterOffset = 20;

		// Left
		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() - birdCenterOffset - birdThickness,
				center.getY() - birdThickness / 2,
				birdThickness,
				birdHeight
			),
			&Theme::bg1
		);

		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() - birdCenterOffset - birdThickness - birdWidth,
				center.getY() - birdThickness / 2,
				birdWidth,
				birdThickness
			),
			&Theme::bg1
		);

		// Right
		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() + birdCenterOffset,
				center.getY() - birdThickness / 2,
				birdThickness,
				birdHeight
			),
			&Theme::bg1
		);

		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() + birdCenterOffset + birdThickness,
				center.getY() - birdThickness / 2,
				birdWidth,
				birdThickness
			),
			&Theme::bg1
		);

		// Dot
		screenBuffer->renderFilledRectangle(
			Bounds(
				center.getX() + birdThickness / 2 - birdThickness / 2,
				center.getY() - birdThickness / 2,
				birdThickness,
				birdThickness
			),
			&Theme::bg1
		);
	}

	void PFD::renderSyntheticVisionBackground(
		ScreenBuffer* screenBuffer,
		const Bounds& bounds,
		const Point& horizonLeft,
		const Point& horizonRight,
		const Point& center
	) {
		// Sky
		screenBuffer->renderFilledRectangle(
			bounds,
			&Theme::sky
		);

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

	void PFD::renderSyntheticVisionVerticalOverlay(
		ScreenBuffer* screenBuffer,
		float unfoldedFOVHeight,
		const Point& horizonLeft,
		const Point& horizonRight
	) {
		// Middle
		screenBuffer->renderLine(
			horizonLeft,
			horizonRight,
			&Theme::fg1
		);

		// Non-middle
		const float lineSmall = 14;
		const float lineBig = 20;
		const uint8_t lineAngleStepDeg = 5;
		const float lineAngleStepRad = radians(lineAngleStepDeg);
		const float linesInTotal = floor(((float) HALF_PI) / lineAngleStepRad);
		const float linePixelStep = (float) unfoldedFOVHeight / linesInTotal;

		const auto& horizonVec = (Vector2F) (horizonRight - horizonLeft);
		const auto& horizonCenter = (Vector2F) horizonLeft + horizonVec / 2.0f;
		const auto& horizonVecNorm = horizonVec.normalize();
		const auto& horizonVecPerp = horizonVecNorm.perpendicular();

		wchar_t text[10];
		Size textSize;

		for (int32_t lineAngleDeg = -90; lineAngleDeg <= 90; lineAngleDeg += lineAngleStepDeg) {
			if (lineAngleDeg == 0)
				continue;

			float lineSize = lineAngleDeg % 10 == 0 ? lineBig : lineSmall;
			float lineY = (float) lineAngleDeg / (float) lineAngleStepDeg * linePixelStep;

			const auto& lineCenterVerticalPerp = horizonCenter + horizonVecPerp * lineY;
			const auto& lineVec = horizonVecNorm * lineSize;
			const auto& lineLeft = (Point) (lineCenterVerticalPerp - lineVec);
			const auto& lineRight = (Point) (lineCenterVerticalPerp + lineVec);

			screenBuffer->renderLine(
				lineLeft,
				lineRight,
				&Theme::fg1
			);

			if (lineAngleDeg % 10 == 0) {
				swprintf(text, 10, L"%d", -lineAngleDeg);
				textSize = Theme::font.getSize(text);

//				screenBuffer->renderText(
//					Point(
//						lineLeft.getX() - textSize.getWidth() - 8,
//						lineLeft.getY() - textSize.getHeight() / 2
//					),
//					&Theme::font,
//					&Theme::fg1,
//					text
//				);

				screenBuffer->renderText(
					Point(
						lineRight.getX() + 8,
						lineRight.getY() - textSize.getHeight() / 2
					),
					&Theme::font,
					&Theme::fg1,
					text
				);
			}
		}
	}

	void PFD::renderSyntheticVisionLateralOverlay(
		ScreenBuffer* screenBuffer,
		const Bounds& bounds
	) {
		auto& app = RCApplication::getInstance();

		const float radius = (float) bounds.getWidth() * 0.8f;
		const auto visiblePartFromBottom = 50;

		const auto& center = Point(
			bounds.getXCenter(),
			bounds.getY2() + (int32_t) radius - visiblePartFromBottom
		);

		const uint8_t lineLength = 5;
		const uint8_t textOffset = 5;

		const float angleStep = radians(5);
		const float yaw = app.getYawInterpolator().getValue();
		const float yawClosestInteger = floor(yaw / angleStep) * angleStep;
		const float yawClosestFractional = yaw - yawClosestInteger;

		const int8_t linesLeftRightCount = 30;
		float angle;

		wchar_t text[8];
		Size textSize;

		for (int8_t i = -linesLeftRightCount; i <= linesLeftRightCount; i++) {
			angle = (float) i * angleStep;

			auto lineToVec = Vector2F(0, -radius).rotate(angle - yawClosestFractional);
			auto lineToVecNorm = lineToVec.normalize();

			auto lineTo = center + (Point) lineToVec;
			auto lineFrom = lineTo + (Point) (lineToVecNorm * -lineLength);

			// Line
			screenBuffer->renderLine(
				lineFrom,
				lineTo,
				&Theme::fg1
			);

			// Text
			auto displayAngle = (int16_t) roundf((float) degrees(yawClosestInteger + angle));

			if (displayAngle < 0)
				displayAngle += 360;

			Serial.printf("Display angle: %d\n", displayAngle);

			if (displayAngle % 10 == 0) {
				swprintf(text, 8, L"%d", displayAngle);
				textSize = Theme::font.getSize(text);

				auto lineToText = lineTo + (Point) (lineToVecNorm * textOffset);

				screenBuffer->renderText(
					Point(
						lineToText.getX() - textSize.getWidth() / 2,
						lineToText.getY() - textSize.getHeight()
					),
					&Theme::font,
					&Theme::fg1,
					text
				);
			}
		}
	}

	void PFD::renderSyntheticVision(ScreenBuffer* screenBuffer, const Bounds& bounds) {
		auto& app = RCApplication::getInstance();

		const auto& center = bounds.getCenter();

		const auto pitch = app.getPitchInterpolator().getValue();
		const auto roll = app.getRollInterpolator().getValue();

		// value = [180 deg of unfolded full range view] / [FOV deg of camera viewport] * [viewport size in pixels]
		const float unfoldedFovWidth = PI / _horizontalFov * bounds.getWidth();
		const float unfoldedFovHeight = PI / _verticalFov * bounds.getHeight();

		const auto& horizonRollRotated = (Point) Vector2F(unfoldedFovWidth, 0).rotate(roll);
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
			horizonRight,
			center
		);

		// Vertical overlay
		renderSyntheticVisionVerticalOverlay(
			screenBuffer,
			unfoldedFovHeight,
			horizonLeft,
			horizonRight
		);

		// Lateral overlay
		renderSyntheticVisionLateralOverlay(
			screenBuffer,
			bounds
		);

		// Bird
		renderSyntheticVisionBird(screenBuffer, center);

		// Temp blyad radio
		wchar_t text[127];

		swprintf(text, 255, L"RSSI: %.2f dBm", app.getTransceiver().getRssi());
		screenBuffer->renderText(
			Point(bounds.getX() + 10, bounds.getY() + 10),
			&Theme::font,
			&Theme::fg1,
			text
		);

		swprintf(text, 255, L"SNR: %.2f dB", app.getTransceiver().getSnr());
		screenBuffer->renderText(
			Point(bounds.getX() + 10, bounds.getY() + 20),
			&Theme::font,
			&Theme::fg1,
			text
		);
	}

	void PFD::renderAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) const {
		auto &app = RCApplication::getInstance();

		auto centerY = bounds.getYCenter();
		auto x = bounds.getX();

		screenBuffer->renderFilledRectangle(bounds, &Theme::bg2);

		float altitude = app.getAltitudeInterpolator().getValue();
		float snapped = altitude / (float) altitudeStepUnits;
		float snappedInteger = floor(snapped);
		float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) altitudeUnitPixels);
		auto yFullLines = (int32_t) ceil((float) y / (float) altitudeUnitPixels);
		y = y - yFullLines * altitudeUnitPixels;

		int32_t lineValue = (int32_t) (snappedInteger + 1) * altitudeStepUnits + yFullLines * altitudeStepUnits;

		wchar_t text[10];
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
				swprintf(text, 10, L"%d", lineValue);
				textSize = Theme::font.getSize(text);

				screenBuffer->renderText(
					Point(x + lineSizeBig + 5, y - textSize.getHeight() / 2),
					&Theme::font,
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

	void PFD::renderVerticalSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) const {
		auto &app = RCApplication::getInstance();
		auto centerY = bounds.getYCenter();

		// Background
		screenBuffer->renderFilledRectangle(
			bounds,
			&Theme::bg3
		);

		// Lines
		auto lineColor = &Theme::fg4;

		int32_t y = centerY;
		int32_t lineValue = 0;

		Size textSize;
		wchar_t text[10];
		bool isBig;

		auto renderLines = [&](int32_t yAdder) {
			for (; lineValue <= verticalSpeedStepUnitsLimit; lineValue += verticalSpeedStepUnits) {
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

					swprintf(text, 10, L"%d", lineValue / 100);
					textSize = Theme::font.getSize(text);

					screenBuffer->renderText(
						Point(
							bounds.getX() + lineSizeBig + 4,
							y - textSize.getHeight() / 2
						),
						&Theme::font,
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

	void PFD::renderMiniPanel(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color *bg, const Color *fg, wchar_t* buffer, int8_t textXOffset) const {
		auto &app = RCApplication::getInstance();

		// Background
		screenBuffer->renderFilledRectangle(bounds, bg);

		// Text
		auto textSize = Theme::font.getSize(buffer);

		screenBuffer->renderText(
			Point(
				bounds.getX() + textXOffset + (bounds.getWidth() - textXOffset) / 2 - textSize.getWidth() / 2,
				bounds.getY() + miniHeight / 2 - textSize.getHeight() / 2
			),
			&Theme::font,
			fg,
			buffer
		);
	}

	void PFD::renderMiniPanelWithAutopilotValue(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color *bg, const Color *fg, float autopilotValue, bool left) const {
		auto &app = RCApplication::getInstance();

		wchar_t text[8];

		if (autopilotValue > 0) {
			swprintf(text, 8, L"%.0f", autopilotValue);
		}
		else {
			swprintf(text, 8, L"----");
		}

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

	void PFD::renderAutopilotSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) const {
		auto &app = RCApplication::getInstance();

		auto bg = &Theme::bg3;
		auto fg = &Theme::blue;

		renderMiniPanelWithAutopilotValue(screenBuffer, bounds, bg, fg, app.getLocalData().getAutopilotSpeed(), true);
	}

	void PFD::renderAutopilotAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) const {
		auto &app = RCApplication::getInstance();

		auto bg = &Theme::bg3;
		auto fg = &Theme::blue;

		renderMiniPanelWithAutopilotValue(screenBuffer, bounds, bg, fg, app.getLocalData().getAutopilotAltitude(), false);
	}

	void PFD::renderPressure(ScreenBuffer* screenBuffer, const Bounds& bounds) const {
		auto &app = RCApplication::getInstance();

		auto bg = &Theme::bg3;
		auto fg = &Theme::blue;

		wchar_t text[9];

		switch (app.getLocalData().getAltimeterMode()) {
			case QNH:
				swprintf(text, 9, L"%d", (uint16_t) app.getLocalData().getAltimeterPressure());

				break;

			case QNE:
				swprintf(text, 9, L"STD");
				bg = &Theme::yellow;
				fg = &Theme::bg1;

				break;
		}

		renderMiniPanel(screenBuffer, bounds, bg, fg, text, 0);
	}

	void PFD::onRender(ScreenBuffer* screenBuffer) {
		auto &bounds = getBounds();

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