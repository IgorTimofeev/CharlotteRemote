#pragma once

#include "../../theme.h"
#include "../../../../lib/YOBA/src/hardware/screen/buffers/screenBuffer.h"
#include "../../../../lib/YOBA/src/ui/element.h"

using namespace yoba;

namespace pizdanc {
	class PFD : public Element {
		public:
			PFD();

			void onRender(ScreenBuffer* screenBuffer) override;

		private:
			const uint16_t lineSizeBig = 5;
			const uint16_t lineSizeSmall = 2;

			const uint8_t miniHeight = 16;

			const uint16_t currentValueHeight = 20;
			const uint8_t currentValueTriangleSize = 8;

			// Speed
			const uint8_t speedWidth = 32;
			const uint8_t speedStepPixels = 8;
			const uint8_t speedStepUnits = 1;
			const uint8_t speedStepUnitsBig = 5;

			const uint16_t speedBarSize = 4;

			const uint16_t speedFlapsMin = 5;
			const uint16_t speedFlapsMax = 30;

			const uint16_t speedTurbulentMin = 10;
			const uint16_t speedTurbulentMax = 80;

			const uint16_t speedSmoothMax = 100;

			// Altitude
			const uint8_t altitudeWidth = 32;
			const uint8_t altitudeStepUnits = 2;
			const uint8_t altitudeStepUnitsBig = 10;
			const uint8_t altitudeUnitPixels = 8;

			// Vertical speed
			const uint8_t verticalSpeedWidth = 16;
			const uint16_t verticalSpeedStepUnits = 25;
			const uint16_t verticalSpeedStepUnitsLimit = 200;
			const uint16_t verticalSpeedStepUnitsBig = 100;
			const uint16_t verticalSpeedStepPixels = 11;
			const uint16_t verticalSpeedStepPixelsRight = 2;

			// Autopilot indicator
			const uint8_t autopilotIndicatorWidth = 4;
			const uint8_t autopilotIndicatorHeight = miniHeight;
			const uint8_t autopilotIndicatorHeightHalf = autopilotIndicatorHeight / 2;

			const uint8_t autopilotIndicatorTriangleVerticalMargin = 3;
			const uint8_t autopilotIndicatorTriangleWidth = 3;
			const uint8_t autopilotIndicatorTriangleHeight = autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin * 2;

			const uint8_t autopilotIndicatorRectangleWidth = autopilotIndicatorWidth - autopilotIndicatorTriangleWidth;

			// 3D
			float _horizontalFov = radians(150);
			float _verticalFov = radians(130);

			void renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Point& point, bool left) const;

			void renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, float autopilotValue, bool left) const;

			void renderCurrentValue(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, float currentValue, bool left) const;

			static void renderTrendArrow(ScreenBuffer* screenBuffer, int32_t x, int32_t y, uint8_t unitStep, uint16_t unitPixels, float value);

			void renderSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) const;

			void renderSyntheticVision(ScreenBuffer* screenBuffer, const Bounds& bounds);

			void renderAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) const;

			void renderVerticalSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) const;

			void renderMiniPanel(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color *bg, const Color *fg, wchar_t *buffer, int8_t textXOffset) const;

			void renderPressure(ScreenBuffer* screenBuffer, const Bounds& bounds) const;

			void renderAutopilotSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) const;

			void renderAutopilotAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) const;

			void renderMiniPanelWithAutopilotValue(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color* bg, const Color* fg, float autopilotValue, bool left) const;

			static void renderSyntheticVisionBird(ScreenBuffer* screenBuffer, const Point& center);

			static void renderSyntheticVisionPitchOverlay(ScreenBuffer* screenBuffer, const Bounds& bounds, float unfoldedFOVHeight, const Point& horizonLeft, const Point& horizonRight);

			static void renderSyntheticVisionBackground(ScreenBuffer* screenBuffer, const Bounds& bounds, const Point& horizonLeft, const Point& horizonRight, const Point& center);

			static void renderSyntheticVisionYawOverlay(ScreenBuffer* screenBuffer, const Bounds& bounds);
	};
}