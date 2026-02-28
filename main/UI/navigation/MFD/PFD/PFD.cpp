#include "PFD.h"

#include <numbers>

#include <units.h>

#include "rc.h"

namespace pizda {
	PFD::PFD() {
		setClipToBounds(true);

		_scene.setMargin(Margin(speedWidth, 0, altitudeWidth + verticalSpeedWidth, 0));
		*this += &_scene;

		_splitter.setSize(Size(speedWidth, miniHeight));
		_splitter.setAlignment(Alignment::start, Alignment::end);
		*this += &_splitter;
	}

	void PFD::onTick() {
		Element::onTick();

		auto& rc = RC::getInstance();

		_scene.setCameraPosition(rc.getAircraftData().computed.coordinates.toCartesian());

		_scene.setWorldRotation(Vector3F(
			toRadians(90) - rc.getAircraftData().computed.coordinates.getLatitude(),
			0,
			toRadians(90) + rc.getAircraftData().computed.coordinates.getLongitude()
		));

		_scene.setCameraRotation(Vector3F(
			rc.getAircraftData().computed.pitchRad,
			rc.getAircraftData().computed.rollRad,
			rc.getAircraftData().computed.yawRad
		));

		invalidate();
	}

	void PFD::onRender(Renderer* renderer, const Bounds& bounds) {
		Layout::onRender(renderer, bounds);

		renderSpeed(renderer, Bounds(
			bounds.getX(),
			bounds.getY() + miniHeight,
			speedWidth,
			bounds.getHeight() - miniHeight * 2
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

	void PFD::renderCurrentValue(Renderer* renderer, const Bounds& bounds, const uint8_t digitCount, float value, const bool left) {
		const auto isConnected = RC::getInstance().getTransceiver().isConnected();
		const auto bg = isConnected ? &Theme::bg2 : &Theme::bad3;
		const auto x2 = bounds.getX2();
		const auto yCenter = bounds.getYCenter();
		
		// Rect
		renderer->renderFilledRectangle(
			Bounds(
				left ? bounds.getX() : bounds.getX() + currentValueTriangleSize,
				bounds.getY(),
				bounds.getWidth() - currentValueTriangleSize,
				bounds.getHeight()
			),
			bg
		);
		
		// Triangle
		renderer->renderFilledTriangle(
			Point(
				left ? x2 - currentValueTriangleSize + 1 : bounds.getX() + currentValueTriangleSize - 1,
				bounds.getY()
			),
			Point(
				left ? x2 : bounds.getX(),
				yCenter
			),
			Point(
				left ? x2 - currentValueTriangleSize + 1 : bounds.getX() + currentValueTriangleSize - 1,
				bounds.getY2()
			),
			bg
		);

		// Text
		const auto textY = yCenter - currentValueFont->getHeight() / 2;
		
		// Assuming 4 is "widest" digit

		if (isConnected) {
			const auto oldViewport = renderer->pushViewport(bounds);
			
			value = std::abs(value);
			
			const uint8_t maxDigitWidth = currentValueFont->getWidth(L'4');
			
			int32_t textX =
				left
				? x2 - speedBarSize - currentValueTextOffset - maxDigitWidth
				: bounds.getX() + currentValueTextOffset + maxDigitWidth * (digitCount - 1) + 1;
			
			const auto getAdjacentDigit = [&](const uint8_t digit, const bool plus) {
				return
					plus
					? (digit < 9 ? digit + 1 : 0)
					: (digit > 1 ? digit - 1 : 9);
			};
			
			const auto renderDigit = [&](const int32_t digitY, const uint8_t digit) {
				const wchar_t text = L'0' + digit;
				
				renderer->renderChar(
					Point(
						textX,
						digitY
					),
					currentValueFont,
					&Theme::fg1,
					text
				);
			};
			
			for (uint8_t digitIndex = 0; digitIndex < digitCount; ++digitIndex) {
				const auto digit = static_cast<uint32_t>(value) % 10;
				
				// Roll
				if (digitIndex == 0) {
					float integer;
					const auto fractional = std::modf(value, &integer);
					const auto rolledY = textY + static_cast<int32_t>(fractional * static_cast<float>(currentValueFont->getHeight()));
					
					renderDigit(rolledY - currentValueFont->getHeight(), getAdjacentDigit(digit, true));
					renderDigit(rolledY, digit);
					renderDigit(rolledY + currentValueFont->getHeight(), getAdjacentDigit(digit, false));
				}
				else {
					renderDigit(textY, digit);
				}
				
				textX -= maxDigitWidth;
				value = value / 10;
			}
			
			renderer->popViewport(oldViewport);
		}
		else {
			const auto text = L"---";
			
			renderer->renderString(
				Point(
					left
						? x2 - speedBarSize - currentValueTextOffset - Theme::fontSmall.getWidth(text)
						: bounds.getX() + speedBarSize + currentValueTextOffset,
					textY
				),
				&Theme::fontSmall,
				&Theme::bad1,
				text
			);
		}
	}
	
	void PFD::renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, const int32_t centerY, const uint8_t unitStep, const uint16_t stepPixels, const float currentValue, const uint16_t autopilotValue, const bool left) {
		if (autopilotValue == 0)
			return;
		
		const auto indicatorCenterY = std::clamp<int32_t>(
			centerY
				- static_cast<int32_t>(
					(static_cast<float>(autopilotValue) - currentValue)
					* static_cast<float>(stepPixels)
					/ static_cast<float>(unitStep)
				),
			bounds.getY(),
			bounds.getY2()
		);
		
		const auto x = left ? bounds.getX2() + 1 - autopilotIndicatorThickness : bounds.getX();
		const auto y= indicatorCenterY - autopilotIndicatorSize / 2;
				
		// Common rect
		renderer->renderFilledRectangle(
			Bounds(
				left ? x + autopilotIndicatorTriangleThickness : x,
				y,
				autopilotIndicatorRectangleThickness,
				autopilotIndicatorSize
			),
			&Theme::ocean
		);
		
		// Upper rect
		renderer->renderFilledRectangle(
			Bounds(
				left ? x : x + autopilotIndicatorRectangleThickness,
				y,
				autopilotIndicatorThickness - autopilotIndicatorRectangleThickness,
				autopilotIndicatorTriangleMargin
			),
			&Theme::ocean
		);
		
		// Lower rect
		renderer->renderFilledRectangle(
			Bounds(
				left ? x : x + autopilotIndicatorRectangleThickness,
				y + autopilotIndicatorSize - autopilotIndicatorTriangleMargin,
				autopilotIndicatorThickness - autopilotIndicatorRectangleThickness,
				autopilotIndicatorTriangleMargin
			),
			&Theme::ocean
		);
		
		// Upper triangle
		renderer->renderFilledTriangle(
			Point(
				left ? x : x + autopilotIndicatorRectangleThickness,
				y + autopilotIndicatorTriangleMargin
			),
			Point(
				left ? x + autopilotIndicatorTriangleThickness - 1 : x + autopilotIndicatorThickness - 1,
				y + autopilotIndicatorTriangleMargin
			),
			Point(
				left ? x + autopilotIndicatorTriangleThickness - 1 : x + autopilotIndicatorRectangleThickness - 1,
				y + autopilotIndicatorSize / 2
			),
			&Theme::ocean
		);
		
		// Lower triangle
		renderer->renderFilledTriangle(
			Point(
				left ? x + autopilotIndicatorTriangleThickness - 1 : x + autopilotIndicatorRectangleThickness,
				y + autopilotIndicatorSize / 2
			),
			Point(
				left ? x + autopilotIndicatorTriangleThickness - 1 : x + autopilotIndicatorThickness - 1,
				y + autopilotIndicatorSize - autopilotIndicatorTriangleMargin - 1
			),
			Point(
				left ? x : x + autopilotIndicatorRectangleThickness - 1,
				y + autopilotIndicatorSize - autopilotIndicatorTriangleMargin - 1
			),
			&Theme::ocean
		);
	}

	void PFD::renderTrendArrow(Renderer* renderer, const int32_t x, const int32_t y, const uint8_t unitStep, const uint16_t stepPixels, const float value) {
		const auto length = static_cast<int32_t>(static_cast<float>(stepPixels) * value / static_cast<float>(unitStep));

		if (std::abs(length) < unitStep)
			return;

		constexpr uint8_t arrowSize = 3;

		const auto yArrow = y - length - arrowSize;
		const auto yMin = std::min(y, yArrow);
		const auto yMax = std::max(y, yArrow);

		renderer->renderVerticalLine(
			Point(x, yMin),
			yMax - yMin,
			&Theme::magenta1
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
			&Theme::magenta1
		);
	}

	void PFD::renderSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		const auto oldViewport = renderer->pushViewport(bounds);

		const auto centerY = bounds.getYCenter();

		renderer->renderFilledRectangle(bounds, &Theme::bg1);

		// Bars
		const auto barX = bounds.getX2() + 1 - speedBarSize;

		const auto renderBar = [&](const int32_t x, const uint16_t width, const uint16_t fromSpeed, const uint16_t toSpeed, const Color* color) {
			const int32_t fromY = centerY - static_cast<int32_t>((static_cast<float>(fromSpeed) - rc.getAircraftData().computed.airspeedKt) * static_cast<float>(speedStepPixels) / static_cast<float>(speedStepUnits));
			const int32_t height = (toSpeed - fromSpeed) * speedStepPixels / speedStepUnits;

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
			speedFlapsMax,
			&Theme::fg1
		);

		renderBar(
			barX,
			speedBarSize,
			speedSmoothMin,
			speedSmoothMax,
			&Theme::green2
		);

		renderBar(
			barX,
			speedBarSize,
			speedTurbulentMin,
			speedTurbulentMax,
			&Theme::yellow
		);

		renderBar(
			barX,
			speedBarSize,
			speedStructuralMin,
			speedStructuralMax,
			&Theme::red
		);

		// Lines
		const float snapped = rc.getAircraftData().computed.airspeedKt / static_cast<float>(speedStepUnits);
		const float snappedInteger = std::floorf(snapped);
		const float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - static_cast<uint16_t>((1.0f - snappedFractional) * static_cast<float>(speedStepPixels));
		const auto altitudeYFullLines = static_cast<int32_t>(std::ceil(static_cast<float>(y) / static_cast<float>(speedStepPixels)));
		y = y - altitudeYFullLines * speedStepPixels;

		int32_t lineValue = static_cast<int32_t>(snappedInteger + 1) * speedStepUnits + altitudeYFullLines * speedStepUnits;

		const Color* lineColor = &Theme::fg3;

		do {
			const auto isBig = lineValue % speedStepUnitsBig == 0;

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
						bounds.getX2() + 1 - speedBarSize - lineSizeBig - lineTextOffset - currentValueFont->getWidth(text),
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
			rc.getAircraftData().computed.airspeedTrendKt
		);

		// V-speeds
		for (const auto& bug : speedBugs) {
			const auto& bugBounds = Bounds(
				bounds.getX2() + 1 + speedBugOffset + speedBugTriangleWidth,
				centerY - static_cast<int32_t>((static_cast<float>(bug.getValue()) - rc.getAircraftData().computed.airspeedKt) * static_cast<float>(speedStepPixels) / static_cast<float>(speedStepUnits)),
				Theme::fontSmall.getWidth(bug.getName()) + speedBugTextOffset * 2,
				Theme::fontSmall.getHeight() + speedBugTextOffset * 2
			);

			// Rect
			renderer->renderFilledRectangle(
				bugBounds,
				1,
				&Theme::bg2
			);

			// Triangle
			renderer->renderFilledTriangle(
				Point(bugBounds.getX(), bugBounds.getY()),
				Point(bugBounds.getX(), bugBounds.getY2()),
				Point(bugBounds.getX() - speedBugTriangleWidth, bugBounds.getYCenter()),
				&Theme::bg2
			);

			// Text
			renderer->renderString(
				Point(bugBounds.getX() + speedBugTextOffset, bugBounds.getY() + speedBugTextOffset),
				&Theme::fontSmall,
				&Theme::green1,
				bug.getName()
			);
		}

		// Autopilot
		renderAutopilotValueIndicator(
			renderer,
			bounds,
			centerY,
			speedStepUnits,
			speedStepPixels,
			rc.getAircraftData().computed.airspeedKt,
			rc.getSettings().autopilot.speedKt,
			true
		);

		// Current speed
		renderCurrentValue(
			renderer,
			Bounds(
				bounds.getX(),
				centerY - currentValueHeight / 2,
				bounds.getWidth(),
				currentValueHeight
			),
			speedMaximumDigits,
			rc.getAircraftData().computed.airspeedKt,
			true
		);

		renderer->popViewport(oldViewport);
	}

	void PFD::renderAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		const auto centerY = bounds.getYCenter();
		auto x = bounds.getX();
		const auto x2 = bounds.getX2();

		renderer->renderFilledRectangle(bounds, &Theme::bg1);

		const float altitude = rc.getAircraftData().computed.altitudeFt;
		const float snapped = altitude / static_cast<float>(altitudeStepUnits);
		const float snappedInteger = std::floorf(snapped);
		const float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - static_cast<uint16_t>((1.0f - snappedFractional) * static_cast<float>(altitudeStepPixels));
		const auto yFullLines = static_cast<int32_t>(std::ceilf(static_cast<float>(y) / static_cast<float>(altitudeStepPixels)));
		y = y - yFullLines * altitudeStepPixels;

		int32_t lineValue = static_cast<int32_t>(snappedInteger + 1) * altitudeStepUnits + yFullLines * altitudeStepUnits;

		const Color* lineColor = &Theme::fg3;

		do {
			const auto isBig = lineValue % altitudeStepUnitsBig == 0;

			if (isBig) {
				renderer->renderHorizontalLine(
					Point(x, y),
					lineSizeBig,
					lineColor
				);

				// Text
				renderer->renderString(
					Point(x + lineSizeBig + lineTextOffset, y - currentValueFont->getHeight() / 2),
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
//			renderer->renderHorizontalLine(
//				Point(x, y),
//				bounds.getWidth(),
//				&Theme::yellow
//			);
//
			constexpr int8_t groundSpacing = 5;
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
				if (groundPoint2.getX() < x2) {
					groundPoint2.setX(groundPoint2.getX() + groundSpacing);
				}
				else {
					groundPoint2.setY(groundPoint2.getY() + groundSpacing);
				}
			}
			while (groundPoint1.getX() < x2 - groundSpacing);
		}

		// Trend
		renderTrendArrow(
			renderer,
			bounds.getX() + lineSizeBig,
			centerY,
			altitudeStepUnits,
			altitudeStepPixels,
			rc.getAircraftData().computed.altitudeTrendFt
		);

		// ALT hold
		if (rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::alt) {
			constexpr static uint8_t horizontalOffset = 5;
			constexpr static uint8_t dashLength = 2;
			constexpr static uint8_t arrowWidth = 3;
			constexpr static uint8_t arrowHeight = 4;

			x = bounds.getX() - horizontalOffset;

			y =
				centerY
				- static_cast<int32_t>(
					(
						Units::convertDistance(rc.getAircraftData().raw.autopilot.targetAltitudeM, DistanceUnit::meter, DistanceUnit::foot)
						- altitude
					)
					* static_cast<float>(altitudeStepPixels)
					/ static_cast<float>(altitudeStepUnits)
				);

			// Arrow
			renderer->renderLine(
				Point(x, y - arrowHeight / 2),
				Point(x + arrowWidth - 1, y),
				&Theme::ocean
			);

			renderer->renderLine(
				Point(x, y + arrowHeight / 2),
				Point(x + arrowWidth - 1, y),
				&Theme::ocean
			);

			x += arrowWidth;

			for (; x < x2 - dashLength + 1; x += dashLength * 2) {
				renderer->renderHorizontalLine(Point(x, y), dashLength, &Theme::ocean);
			}
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
			Bounds(
				bounds.getX(),
				centerY - currentValueHeight / 2,
				bounds.getWidth(),
				currentValueHeight
			),
			altitudeMaximumDigits,
			altitude,
			false
		);

		// Metric units
		if (rc.getSettings().personalization.MFD.PFD.metricUnits) {
			constexpr static uint8_t textLength = 6;
			wchar_t text[textLength];
			std::swprintf(text, textLength, L"%dm", static_cast<int32_t>(Units::convertDistance(altitude, DistanceUnit::foot, DistanceUnit::meter)));

			constexpr static uint8_t verticalPanelOffset = 5;

			constexpr static uint8_t horizontalTextOffset = 2;
			constexpr static uint8_t verticalTextOffset = 1;

			const auto panelSize = Size(
				Theme::fontSmall.getWidth(text) + horizontalTextOffset * 2,
				Theme::fontSmall.getHeight() + verticalTextOffset * 2
			);

			const auto panelBounds = Bounds(
				bounds.getX2() - panelSize.getWidth() + 1,
				centerY - currentValueHeight / 2 - verticalPanelOffset - panelSize.getHeight(),
				panelSize.getWidth(),
				panelSize.getHeight()
			);

			renderer->renderFilledRectangle(
				panelBounds,
				&Theme::bg2
			);

			renderer->renderString(
				Point(
					panelBounds.getX() + horizontalTextOffset,
					panelBounds.getY() + verticalTextOffset
				),
				&Theme::fontSmall,
				&Theme::ocean,
				text
			);
		}
	}

	void PFD::renderVerticalSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		const auto centerY = bounds.getYCenter();

		// Background
		renderer->renderFilledRectangle(bounds, &Theme::bg2);

		// Lines
		auto lineColor = &Theme::fg4;

		int32_t y = centerY;
		int32_t lineValue = 0;

		bool isBig;

		auto renderLines = [&lineValue, &isBig, &renderer, lineColor, &bounds, &y](const int32_t yAdder) {
			while (lineValue <= verticalSpeedStepUnitsLimit) {
				isBig = lineValue % verticalSpeedStepUnitsBig == 0;

				if (isBig) {
					renderer->renderHorizontalLine(
						Point(bounds.getX(), y),
						verticalSpeedLineSizeBig,
						lineColor
					);

					renderer->renderString(
						Point(
							bounds.getX() + verticalSpeedLineSizeBig + verticalSpeedLineTextOffset,
							y - verticalSpeedFont->getHeight() / 2
						),
						verticalSpeedFont,
						lineColor,
						std::to_wstring(lineValue / 1000)
					);
				}
				else {
					renderer->renderHorizontalLine(
						Point(bounds.getX(), y),
						verticalSpeedLineSizeSmall,
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
			Point(bounds.getX(), centerY - static_cast<int32_t>(rc.getAircraftData().computed.verticalSpeedFtPM * static_cast<float>(verticalSpeedStepPixels) / static_cast<float>(verticalSpeedStepUnits))),
			Point(bounds.getX2(), centerY - static_cast<int32_t>(rc.getAircraftData().computed.verticalSpeedFtPM * static_cast<float>(verticalSpeedStepPixelsRight) / static_cast<float>(verticalSpeedStepUnits))),
			&Theme::green1
		);
	}

	void PFD::renderMiniPanel(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, const std::wstring_view text, const int8_t textXOffset) {
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

	void PFD::renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, const uint16_t autopilotValue, const bool autopilotValueEnabled, const bool left) {
		renderMiniPanel(
			renderer,
			bounds,
			bg,
			fg,
			autopilotValueEnabled ? std::to_wstring(autopilotValue) : L"----",
			static_cast<int8_t>(left ? -autopilotIndicatorTriangleThickness : autopilotIndicatorTriangleThickness)
		);
	}

	void PFD::renderAutopilotSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		const auto bg = &Theme::bg2;
		const auto fg = &Theme::ocean;

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.speedKt, rc.getSettings().autopilot.speedKt > 0, true);
	}

	void PFD::renderAutopilotAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		const auto bg = &Theme::bg2;
		const auto fg = &Theme::ocean;

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.altitudeFt, rc.getSettings().autopilot.altitudeFt > 0, false);
	}

	void PFD::renderPressure(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::yellow;

		std::wstring text;

		if (rc.getSettings().ADIRS.referencePressureSTD) {
			text = L"STD";
			bg = &Theme::yellow;
			fg = &Theme::bg1;
		}
		else {
			text = std::to_wstring(static_cast<uint32_t>(
				Units::convertPressure(
					rc.getSettings().ADIRS.referencePressurePa,
					PressureUnit::pascal,
					PressureUnit::hectopascal
				)
			));
		}

		renderMiniPanel(renderer, bounds, bg, fg, text, 0);
	}
}