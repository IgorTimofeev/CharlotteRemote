#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../theme.h"
#include "../../../units.h"
#include "../../../aircraftData.h"

namespace pizda {
	using namespace YOBA;

	class PFD : public SpatialView {
		public:
			PFD();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		protected:
			void onTick() override;

		private:
			constexpr static const uint16_t lineSizeBig = 4;
			constexpr static const uint16_t lineSizeSmall = 2;
			constexpr static const uint16_t lineSizeTextOffset = 4;

			constexpr static const uint8_t miniHeight = 16;
			constexpr static const Font* miniFont = &Theme::fontSmall;

			constexpr static const uint16_t currentValueHeight = 20;
			constexpr static const uint8_t currentValueTriangleSize = 8;
			constexpr static const Font* currentValueFont = &Theme::fontSmall;

			// Speed
			constexpr static const uint8_t speedWidth = 29;
			constexpr static const uint8_t speedStepPixels = 8;
			constexpr static const uint8_t speedStepUnits = 2;
			constexpr static const uint8_t speedStepUnitsBig = 10;

			constexpr static const uint16_t speedBarSize = 4;

			constexpr static const uint16_t speedFlapsMin = 40;
			constexpr static const uint16_t speedFlapsMax = 85;

			constexpr static const uint16_t speedSmoothMin = 47;
			constexpr static const uint16_t speedSmoothMax = 128;

			constexpr static const uint16_t speedTurbulentMin = speedSmoothMax;
			constexpr static const uint16_t speedTurbulentMax = 163;

			constexpr static const uint16_t speedStructuralMin = speedTurbulentMax;
			constexpr static const uint16_t speedStructuralMax = speedStructuralMin * 4;

			// V-speeds
			constexpr static const uint8_t VSpeedMargin = 4;
			constexpr static const uint8_t VSpeedTextOffset = 2;
			constexpr static const uint8_t VSpeedTriangleWidth = 4;

			constexpr static const VSpeed VSpeeds[] = {
				VSpeed(L"Y", 74),
				VSpeed(L"G", 68),
				VSpeed(L"R", 55)
			};

			// Altitude
			constexpr static const uint8_t altitudeWidth = 30;
			constexpr static const uint8_t altitudeStepUnits = 20;
			constexpr static const uint8_t altitudeStepUnitsBig = 100;
			constexpr static const uint8_t altitudeStepPixels = 8;

			constexpr static const uint8_t altitudeMinimumHorizontalOffset = 5;
			constexpr static const uint8_t altitudeMinimumTriangleWidth = 3;
			constexpr static const uint8_t altitudeMinimumTriangleHeight = 3;
			constexpr static const uint8_t altitudeMinimumSafeUnitDelta = 50;

			// Vertical speed
			constexpr static const uint8_t verticalSpeedWidth = 13;
			constexpr static const uint16_t verticalSpeedStepUnits = 250;
			constexpr static const uint16_t verticalSpeedStepUnitsLimit = 2000;
			constexpr static const uint16_t verticalSpeedStepUnitsBig = 1000;
			constexpr static const uint16_t verticalSpeedStepPixels = 11;
			constexpr static const uint16_t verticalSpeedStepPixelsRight = 2;
			constexpr static const uint16_t verticalSpeedTextOffset = 3;

			constexpr static const Font* verticalSpeedFont = &Theme::fontSmall;

			// Autopilot indicator
			constexpr static const uint8_t autopilotIndicatorWidth = 4;
			constexpr static const uint8_t autopilotIndicatorHeight = miniHeight;
			constexpr static const uint8_t autopilotIndicatorHeightHalf = autopilotIndicatorHeight / 2;

			constexpr static const uint8_t autopilotIndicatorTriangleVerticalMargin = 3;
			constexpr static const uint8_t autopilotIndicatorTriangleWidth = 3;
			constexpr static const uint8_t autopilotIndicatorTriangleHeight = autopilotIndicatorHeight - autopilotIndicatorTriangleVerticalMargin * 2;

			constexpr static const uint8_t autopilotIndicatorRectangleWidth = autopilotIndicatorWidth - autopilotIndicatorTriangleWidth;

			// Pitch overlay
			constexpr static const uint8_t pitchOverlayMarginTop = 30;
			constexpr static const uint8_t pitchOverlayAngleStepDeg = 5;
			constexpr static const uint8_t pitchOverlayLineSmall = 10;
			constexpr static const uint8_t pitchOverlayLineBig = 20;
			constexpr static const uint8_t pitchOverlayTextOffset = 5;

			constexpr static const Font* pitchOverlayFont = &Theme::fontSmall;
			constexpr static const Color* pitchOverlayColorSky = &Theme::sky2;
			constexpr static const Color* pitchOverlayColorGround = &Theme::ground2;

			// Yaw overlay
			constexpr static const uint8_t yawOverlayHeight = 20;
			constexpr static const uint8_t yawOverlayAngleStepUnits = 2;
			constexpr static const uint8_t yawOverlayAngleStepPixels = 9;
			constexpr static const uint8_t yawOverlayLineSmallLength = 2;
			constexpr static const uint8_t yawOverlayLineBigLength = 4;
			constexpr static const uint8_t yawOverlayTextOffset = 0;

			constexpr static const uint8_t yawOverlayTriangleWidth = 8;
			constexpr static const uint8_t yawOverlayTriangleHeight = yawOverlayLineBigLength;

			constexpr static const Font* yawOverlayFont = &Theme::fontSmall;
			constexpr static const Color* yawOverlayColor = &Theme::ground2;

			// Turn coordinator overlay
			constexpr static const Color* turnCoordinatorOverlayColor = &Theme::sky2;
			constexpr static const uint8_t turnCoordinatorOverlayRollIndicatorRadius = 100;

			constexpr static const uint8_t turnCoordinatorOverlayRollIndicatorLineSmallLength = 3;
			constexpr static const uint8_t turnCoordinatorOverlayRollIndicatorLineBigLength = 7;

			constexpr static const uint8_t turnCoordinatorOverlayRollIndicatorTriangleOffset = 2;
			constexpr static const uint8_t turnCoordinatorOverlayRollIndicatorTriangleWidth = 9;
			constexpr static const uint8_t turnCoordinatorOverlayRollIndicatorTriangleHeight = 5;

			constexpr static const uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorOffset = 3;
			constexpr static const uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorWidth = turnCoordinatorOverlayRollIndicatorTriangleWidth;
			constexpr static const uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorHeight = 2;
			constexpr static const uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorMaxValuePixels = 40;

			// Aircraft symbol
			constexpr static const uint8_t aircraftSymbolWidth = 30;
			constexpr static const uint8_t aircraftSymbolThickness = 2;
			constexpr static const uint8_t aircraftSymbolCenterOffset = 20;

			// Flight path vector
			constexpr static const uint8_t flightPathVectorRadius = 4;
			constexpr static const uint8_t flightPathVectorLineLength = 6;
			constexpr static const uint8_t flightPathVectorLineThickness = 2;

			// Flight director
			constexpr static const uint8_t flightDirectorLengthFactor = 50;
			constexpr static const uint8_t flightDirectorThickness = 2;

			// Wind
			constexpr static const uint8_t windVisibilityGroundSpeed = 10;

			static void renderAutopilotValueIndicator(Renderer* renderer, const Point& point, bool left) ;

			static void renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t stepPixels, float currentValue, uint16_t autopilotValue, bool left) ;

			static void renderCurrentValue(Renderer* renderer, const Bounds& bounds, int32_t centerY, float value, bool left);

			static void renderTrendArrow(Renderer* renderer, int32_t x, int32_t y, uint8_t unitStep, uint16_t stepPixels, float value);

			static void renderSpeed(Renderer* renderer, const Bounds& bounds) ;

			void renderSyntheticVision(Renderer* renderer, const Bounds& bounds);

			static void renderAltitude(Renderer* renderer, const Bounds& bounds) ;

			static void renderVerticalSpeed(Renderer* renderer, const Bounds& bounds) ;

			static void renderMiniPanel(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, std::wstring_view text, int8_t textXOffset) ;

			static void renderPressure(Renderer* renderer, const Bounds& bounds) ;

			static void renderAutopilotSpeed(Renderer* renderer, const Bounds& bounds) ;

			static void renderAutopilotAltitude(Renderer* renderer, const Bounds& bounds) ;

			static void renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, uint16_t autopilotValue, bool autopilotValueEnabled, bool left);

			static void renderPitchOverlay(
				Renderer* renderer,
				const Bounds& bounds,
				const AircraftData& ad,
				float pitchPixelOffsetProjected,
				float projectionPlaneDistance,
				const Point& horizonLeft,
				const Point& horizonRight,

				const Vector2F& horizonVec,
				const Vector2F& horizonVecNorm,
				const Vector2F& horizonVecPerp,
				const Vector2F& horizonCenter
			);

			static void renderSyntheticVisionBackground(Renderer* renderer, const Bounds& bounds, const Point& horizonLeft, const Point& horizonRight);

			static void renderTurnCoordinatorOverlay(Renderer* renderer, const Bounds& bounds, const AircraftData& aircraftData);

			static void renderYawOverlay(Renderer* renderer, const Bounds& bounds, const AircraftData& aircraftData);

			void renderGroundSpeed(Renderer* renderer, const Bounds& bounds);

			void renderWind(Renderer* renderer, const Point& bottomLeft) const;
	};
}