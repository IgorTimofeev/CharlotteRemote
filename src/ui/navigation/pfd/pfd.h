#pragma once

#include "../../../../../lib/YOBA/src/hardware/screen/buffers/screenBuffer.h"
#include "../../../../../lib/YOBA/src/ui/element.h"
#include "../../theme.h"

using namespace yoba;

namespace pizdanc {
	class PFD : public Element {
		public:
			PFD();

			void onRender(ScreenBuffer* screenBuffer) override;

		private:
			static const uint16_t lineSizeBig = 5;
			static const uint16_t lineSizeSmall = 2;

			static const uint8_t miniHeight = 16;

			static const uint16_t currentValueHeight = 20;
			static const uint8_t currentValueTriangleSize = 8;

			// Speed
			static const uint8_t speedWidth = 32;
			static const uint8_t speedStepPixels = 8;
			static const uint8_t speedStepUnits = 1;
			static const uint8_t speedStepUnitsBig = 5;

			static const uint16_t speedBarSize = 4;

			static const uint16_t speedFlapsMin = 5;
			static const uint16_t speedFlapsMax = 30;

			static const uint16_t speedTurbulentMin = 10;
			static const uint16_t speedTurbulentMax = 80;

			static const uint16_t speedSmoothMax = 100;

			// Altitude
			static const uint8_t altitudeWidth = 32;
			static const uint8_t altitudeStepUnits = 2;
			static const uint8_t altitudeStepUnitsBig = 10;
			static const uint8_t altitudeUnitPixels = 8;

			// Vertical speed
			static const uint8_t verticalSpeedWidth = 16;
			static const uint16_t verticalSpeedStepUnits = 25;
			static const uint16_t verticalSpeedStepUnitsLimit = 200;
			static const uint16_t verticalSpeedStepUnitsBig = 100;
			static const uint16_t verticalSpeedStepPixels = 11;
			static const uint16_t verticalSpeedStepPixelsRight = 2;

			// Autopilot indicator
			static const uint8_t autopilotIndicatorWidth = 4;
			static const uint8_t autopilotIndicatorHeight = miniHeight;
			static const uint8_t autopilotIndicatorHeightHalf = autopilotIndicatorHeight / 2;

			static const uint8_t autopilotIndicatorTriangleVerticalMargin = 3;
			static const uint8_t autopilotIndicatorTriangleWidth = 3;
			static const uint8_t autopilotIndicatorTriangleHeight = autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin * 2;

			static const uint8_t autopilotIndicatorRectangleWidth = autopilotIndicatorWidth - autopilotIndicatorTriangleWidth;

			// Pitch overlay
			static const Color* pitchOverlayColorSky;
			static const Color* pitchOverlayColorGround;
			static const uint8_t pitchOverlayAngleStep = 5;
			static const uint8_t pitchOverlayLineSmall = 5;
			static const uint8_t pitchOverlayLineBig = 10;
			static const uint8_t pitchOverlayLineMiddle = 15;
			static const uint8_t pitchOverlayTextOffset = 5;

			// Yaw overlay
			static const Color* yawOverlayColor;
			static const uint8_t yawOverlayHeight = 20;
			static const uint8_t yawOverlayAngleStepUnits = 2;
			static const uint8_t yawOverlayAngleStepPixels = 9;
			static const uint8_t yawOverlayLineSmallLength = 2;
			static const uint8_t yawOverlayLineBigLength = 4;
			static const uint8_t yawOverlayTextOffset = 0;
			static const uint8_t yawOverlayTriangleSize = yawOverlayLineBigLength;

			// Roll overlay
			static const Color* rollOverlayColor;
			static const uint8_t rollOverlayHeight = 30;
			static const uint8_t rollOverlayAngleStep = 2;
			static const uint8_t rollOverlayFromToAngle = 30;
			static const uint8_t rollOverlayLinesCount = rollOverlayFromToAngle / rollOverlayAngleStep;
			static const uint8_t rollOverlayLineSmallLength = 2;
			static const uint8_t rollOverlayLineBigLength = 4;
			static const uint8_t rollOverlayTextOffset = 5;
			static const uint8_t rollOverlayTriangleSize = yawOverlayLineBigLength;

			// 3D
			float _horizontalFov = radians(120);
			float _verticalFov = radians(90);

			static void renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Point& point, bool left) ;

			static void renderAutopilotValueIndicator(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, float autopilotValue, bool left) ;

			static void renderCurrentValue(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, float value, bool left) ;

			static void renderTrendArrow(ScreenBuffer* screenBuffer, int32_t x, int32_t y, uint8_t unitStep, uint16_t unitPixels, float value);

			static void renderSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) ;

			void renderSyntheticVision(ScreenBuffer* screenBuffer, const Bounds& bounds) const;

			static void renderAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) ;

			static void renderVerticalSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) ;

			static void renderMiniPanel(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color *bg, const Color *fg, wchar_t *buffer, int8_t textXOffset) ;

			static void renderPressure(ScreenBuffer* screenBuffer, const Bounds& bounds) ;

			static void renderAutopilotSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds) ;

			static void renderAutopilotAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds) ;

			static void renderMiniPanelWithAutopilotValue(ScreenBuffer* screenBuffer, const Bounds& bounds, const Color* bg, const Color* fg, float autopilotValue, bool left) ;

			static void renderSyntheticVisionBird(ScreenBuffer* screenBuffer, const Point& center);

			static void renderPitchOverlay(ScreenBuffer* screenBuffer, const Bounds& bounds, float unfoldedFOVHeight, const Point& horizonLeft, const Point& horizonRight);

			static void renderSyntheticVisionBackground(ScreenBuffer* screenBuffer, const Bounds& bounds, const Point& horizonLeft, const Point& horizonRight);

			static void renderRollOverlay(ScreenBuffer* screenBuffer, const Bounds& bounds, float roll);

			static void renderYawOverlay(ScreenBuffer* screenBuffer, const Bounds& bounds, float yaw);
	};
}