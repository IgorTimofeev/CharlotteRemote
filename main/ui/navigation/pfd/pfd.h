#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"
#include "../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class PFD : public Element {
		public:
			PFD();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			constexpr static uint16_t lineSizeBig = 4;
			constexpr static uint16_t lineSizeSmall = 2;
			constexpr static uint16_t lineSizeTextOffset = 4;

			constexpr static uint8_t miniHeight = 16;

			constexpr static uint16_t currentValueHeight = 20;
			constexpr static uint8_t currentValueTriangleSize = 8;

			// Speed
			constexpr static uint8_t speedWidth = 32;
			constexpr static uint8_t speedStepPixels = 8;
			constexpr static uint8_t speedStepUnits = 1;
			constexpr static uint8_t speedStepUnitsBig = 5;

			constexpr static uint16_t speedBarSize = 4;

			constexpr static uint16_t speedFlapsMin = 5;
			constexpr static uint16_t speedFlapsMax = 30;

			constexpr static uint16_t speedTurbulentMin = 10;
			constexpr static uint16_t speedTurbulentMax = 80;

			constexpr static uint16_t speedSmoothMax = 100;

			// Altitude
			constexpr static uint8_t altitudeWidth = speedWidth;
			constexpr static uint8_t altitudeStepUnits = 2;
			constexpr static uint8_t altitudeStepUnitsBig = 10;
			constexpr static uint8_t altitudeUnitPixels = 8;

			// Vertical speed
			constexpr static uint8_t verticalSpeedWidth = 15;
			constexpr static uint16_t verticalSpeedStepUnits = 25;
			constexpr static uint16_t verticalSpeedStepUnitsLimit = 200;
			constexpr static uint16_t verticalSpeedStepUnitsBig = 100;
			constexpr static uint16_t verticalSpeedStepPixels = 11;
			constexpr static uint16_t verticalSpeedStepPixelsRight = 2;
			constexpr static uint16_t verticalSpeedTextOffset = 3;

			// Autopilot indicator
			constexpr static uint8_t autopilotIndicatorWidth = 4;
			constexpr static uint8_t autopilotIndicatorHeight = miniHeight;
			constexpr static uint8_t autopilotIndicatorHeightHalf = autopilotIndicatorHeight / 2;

			constexpr static uint8_t autopilotIndicatorTriangleVerticalMargin = 3;
			constexpr static uint8_t autopilotIndicatorTriangleWidth = 3;
			constexpr static uint8_t autopilotIndicatorTriangleHeight = autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin * 2;

			constexpr static uint8_t autopilotIndicatorRectangleWidth = autopilotIndicatorWidth - autopilotIndicatorTriangleWidth;

			// Pitch overlay
			constexpr static const Color* pitchOverlayColorSky = &Theme::sky2;
			constexpr static const Color* pitchOverlayColorGround = &Theme::ground2;
			constexpr static uint8_t pitchOverlayMarginTop = 30;
			constexpr static uint8_t pitchOverlayAngleStep = 5;
			constexpr static uint8_t pitchOverlayLineSmall = 5;
			constexpr static uint8_t pitchOverlayLineBig = 10;
			constexpr static uint8_t pitchOverlayLineMiddle = 15;
			constexpr static uint8_t pitchOverlayTextOffset = 5;

			// Yaw overlay
			constexpr static const Color* yawOverlayColor = &Theme::ground2;
			constexpr static uint8_t yawOverlayHeight = 20;
			constexpr static uint8_t yawOverlayAngleStepUnits = 2;
			constexpr static uint8_t yawOverlayAngleStepPixels = 9;
			constexpr static uint8_t yawOverlayLineSmallLength = 2;
			constexpr static uint8_t yawOverlayLineBigLength = 4;
			constexpr static uint8_t yawOverlayTextOffset = 0;
			constexpr static uint8_t yawOverlayTriangleSize = yawOverlayLineBigLength;

			// Roll overlay
			constexpr static const Color* rollOverlayColor = &Theme::sky2;
			constexpr static uint8_t rollOverlayHorizontalMargin = 3;
			constexpr static uint8_t rollOverlayLineSmallLength = 3;
			constexpr static uint8_t rollOverlayLineBigLength = 7;
			constexpr static uint8_t rollOverlayTriangleSize = yawOverlayLineBigLength;

			// 3D
			float _horizontalFov = toRadians(120);
			float _verticalFov = toRadians(90);

			static void renderAutopilotValueIndicator(Renderer* renderer, const Point& point, bool left) ;

			static void renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, float autopilotValue, bool left) ;

			static void renderCurrentValue(Renderer* renderer, const Bounds& bounds, int32_t centerY, float value, bool left) ;

			static void renderTrendArrow(Renderer* renderer, int32_t x, int32_t y, uint8_t unitStep, uint16_t unitPixels, float value);

			static void renderSpeed(Renderer* renderer, const Bounds& bounds) ;

			void renderSyntheticVision(Renderer* renderer, const Bounds& bounds) const;

			static void renderAltitude(Renderer* renderer, const Bounds& bounds) ;

			static void renderVerticalSpeed(Renderer* renderer, const Bounds& bounds) ;

			static void renderMiniPanel(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, std::wstring_view text, int8_t textXOffset) ;

			static void renderPressure(Renderer* renderer, const Bounds& bounds) ;

			static void renderAutopilotSpeed(Renderer* renderer, const Bounds& bounds) ;

			static void renderAutopilotAltitude(Renderer* renderer, const Bounds& bounds) ;

			static void renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, float autopilotValue, bool left) ;

			static void renderAircraftSymbol(Renderer* renderer, const Point& center);

			static void renderPitchOverlay(Renderer* renderer, const Bounds& bounds, float unfoldedFOVHeight, const Point& horizonLeft, const Point& horizonRight);

			static void renderSyntheticVisionBackground(Renderer* renderer, const Bounds& bounds, const Point& horizonLeft, const Point& horizonRight);

			static void renderRollOverlay(Renderer* renderer, const Bounds& bounds, float roll);

			static void renderYawOverlay(Renderer* renderer, const Bounds& bounds, float yaw);
	};
}