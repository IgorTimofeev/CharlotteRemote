#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "UI/theme.h"
#include <units.h>
#include "types/speedBug.h"
#include "types/aircraftData.h"
#include "settings/settings.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class PFDScene : public Scene {
		public:
			PFDScene();

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			static void renderPitchOverlay(
				Renderer* renderer,
				const Bounds& bounds,
				float pitchPixelOffsetProjected,
				float projectionPlaneDistance,
				const Point& horizonLeft,
				const Point& horizonRight,

				const Vector2F& horizonVec,
				const Vector2F& horizonVecNorm,
				const Vector2F& horizonVecPerp,
				const Vector2F& horizonCenter
			);

			static void renderTurnCoordinatorOverlay(Renderer* renderer, const Bounds& bounds);
			static void renderFlightModeAnnunciatorOverlay(Renderer* renderer, const Bounds& bounds);
			static void renderYawOverlay(Renderer* renderer, const Bounds& bounds);
	};

	class PFD : public Layout {
		public:
			PFD();

			constexpr static uint16_t lineSizeBig = 3;
			constexpr static uint16_t lineSizeSmall = 2;
			constexpr static uint16_t lineTextOffset = 3;

			constexpr static uint16_t currentValueHeight = 20;
			constexpr static uint8_t currentValueTriangleSize = 8;
			constexpr static const Font* currentValueFont = &Theme::fontSmall;
			constexpr static uint8_t currentValueTextOffset = 5;

			// Speed
			constexpr static uint8_t speedWidth = 24;
			constexpr static uint8_t speedStepPixels = 8;
			constexpr static uint8_t speedStepUnits = 1;
			constexpr static uint8_t speedStepUnitsBig = 5;
			
			constexpr static uint8_t speedMaximumDigits = 3;
			
			constexpr static uint16_t speedBarSize = 2;

			constexpr static uint16_t speedFlapsMin = 15;
			constexpr static uint16_t speedFlapsMax = 25;

			constexpr static uint16_t speedSmoothMin = 25;
			constexpr static uint16_t speedSmoothMax = 42;

			constexpr static uint16_t speedTurbulentMin = speedSmoothMax;
			constexpr static uint16_t speedTurbulentMax = speedTurbulentMin + 10;

			constexpr static uint16_t speedStructuralMin = speedTurbulentMax;
			constexpr static uint16_t speedStructuralMax = speedStructuralMin * 4;

			// Speed bugs
			constexpr static uint8_t speedBugOffset = 3;
			constexpr static uint8_t speedBugTextOffset = 1;
			constexpr static uint8_t speedBugTriangleWidth = 3;

			constexpr static SpeedBug speedBugs[] = {
				SpeedBug(L"Y", 32)
			};

			// Altitude
			constexpr static uint8_t altitudeWidth = 26;
			constexpr static uint8_t altitudeStepUnits = 10;
			constexpr static uint8_t altitudeStepUnitsBig = 50;
			constexpr static uint8_t altitudeStepPixels = 8;
			
			constexpr static uint8_t altitudeMaximumDigits = 4;
			constexpr static uint8_t altitudeMinimumHorizontalOffset = 5;
			constexpr static uint8_t altitudeMinimumTriangleWidth = 3;
			constexpr static uint8_t altitudeMinimumTriangleHeight = 3;
			constexpr static uint8_t altitudeMinimumSafeUnitDelta = 50;

			// Vertical speed
			constexpr static uint8_t verticalSpeedWidth = 9;
			constexpr static uint16_t verticalSpeedStepUnits = 250;
			constexpr static uint16_t verticalSpeedStepUnitsLimit = 2000;
			constexpr static uint16_t verticalSpeedStepUnitsBig = 1000;
			constexpr static uint16_t verticalSpeedStepPixels = 11;
			constexpr static uint16_t verticalSpeedStepPixelsRight = 2;
			
			constexpr static uint8_t verticalSpeedLineSizeBig = 2;
			constexpr static uint8_t verticalSpeedLineSizeSmall = 1;
			constexpr static uint8_t verticalSpeedLineTextOffset = lineTextOffset;

			constexpr static const Font* verticalSpeedFont = &Theme::fontSmall;

			// Autopilot indicator
			constexpr static uint8_t autopilotIndicatorThickness = 4;
			constexpr static uint8_t autopilotIndicatorSize = 16;

			constexpr static uint8_t autopilotIndicatorTriangleMargin = 3;
			constexpr static uint8_t autopilotIndicatorTriangleThickness = 3;

			constexpr static uint8_t autopilotIndicatorRectangleThickness = autopilotIndicatorThickness - autopilotIndicatorTriangleThickness;
			
			constexpr static uint8_t miniHeight = autopilotIndicatorSize;
			constexpr static const Font* miniFont = &Theme::fontSmall;
			
			// Pitch overlay
			constexpr static uint8_t pitchOverlayVerticalOffset = 15;
			constexpr static uint8_t pitchOverlayAngleStepDeg = 5;
			constexpr static uint8_t pitchOverlayLineSmall = 10;
			constexpr static uint8_t pitchOverlayLineBig = 20;
			constexpr static uint8_t pitchOverlayTextOffset = 5;

			constexpr static const Font* pitchOverlayFont = &Theme::fontSmall;
			constexpr static const Color* pitchOverlayColorSky = &Theme::sky2;
			constexpr static const Color* pitchOverlayColorGround = &Theme::ground2;

			// Yaw overlay
			constexpr static uint8_t yawOverlayHeight = 20;
			constexpr static uint8_t yawOverlayAngleStepUnits = 10;
			constexpr static uint8_t yawOverlayAngleStepUnitsBig = 30;
			constexpr static uint8_t yawOverlayAngleStepPixels = 20;
			constexpr static uint8_t yawOverlayLineSmallLength = 2;
			constexpr static uint8_t yawOverlayLineBigLength = 4;
			constexpr static uint8_t yawOverlayTextOffset = 0;

			constexpr static uint8_t yawOverlayTriangleWidth = 8;
			constexpr static uint8_t yawOverlayTriangleHeight = yawOverlayLineBigLength;

			constexpr static const Font* yawOverlayFont = &Theme::fontSmall;
			constexpr static const Color* yawOverlayColor = &Theme::ground2;

			// Flight mode annunciator
			constexpr static uint8_t flightModeAnnunciatorTopOffset = 1;
			constexpr static uint8_t flightModeAnnunciatorHeight = 10;
			constexpr static uint8_t flightModeAnnunciatorHorizontalOffset = 0;
			
			// Turn coordinator overlay
			constexpr static uint8_t turnCoordinatorOverlayTopOffset = 3;
			constexpr static const Color* turnCoordinatorOverlayColor = &Theme::sky2;
			constexpr static uint8_t turnCoordinatorOverlayRollIndicatorRadius = 100;

			constexpr static uint8_t turnCoordinatorOverlayRollIndicatorLineSmallLength = 3;
			constexpr static uint8_t turnCoordinatorOverlayRollIndicatorLineBigLength = 7;

			constexpr static uint8_t turnCoordinatorOverlayRollIndicatorTriangleOffset = 2;
			constexpr static uint8_t turnCoordinatorOverlayRollIndicatorTriangleWidth = 9;
			constexpr static uint8_t turnCoordinatorOverlayRollIndicatorTriangleHeight = 5;

			constexpr static uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorOffset = 3;
			constexpr static uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorWidth = turnCoordinatorOverlayRollIndicatorTriangleWidth;
			constexpr static uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorHeight = 2;
			constexpr static uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorMaxValuePixels = 40;

			// Aircraft symbol
			constexpr static uint8_t aircraftSymbolWidth = 30;
			constexpr static uint8_t aircraftSymbolThickness = 2;
			constexpr static uint8_t aircraftSymbolCenterOffset = 20;

			// Flight path vector
			constexpr static uint8_t flightPathVectorRadius = 4;
			constexpr static uint8_t flightPathVectorLineLength = 6;
			constexpr static uint8_t flightPathVectorLineThickness = 2;

			// Flight director
			constexpr static uint8_t flightDirectorLengthFactor = 50;
			constexpr static uint8_t flightDirectorThickness = 2;

			// Wind
			constexpr static uint8_t windVisibilityGroundSpeed = 10;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		protected:
			void onTick() override;

		private:
			PFDScene _scene {};

			static void renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t stepPixels, float currentValue, uint16_t autopilotValue, bool left);
			static void renderCurrentValue(Renderer* renderer, const Bounds& bounds, uint8_t digitCount, float value, bool left);
			static void renderTrendArrow(Renderer* renderer, int32_t x, int32_t y, uint8_t unitStep, uint16_t stepPixels, float value);
			static void renderSpeed(Renderer* renderer, const Bounds& bounds);
			static void renderAltitude(Renderer* renderer, const Bounds& bounds);
			static void renderVerticalSpeed(Renderer* renderer, const Bounds& bounds);
			static void renderMiniPanel(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, std::wstring_view text, int8_t textXOffset);
			static void renderPressure(Renderer* renderer, const Bounds& bounds);
			static void renderAutopilotSpeed(Renderer* renderer, const Bounds& bounds);
			static void renderAutopilotAltitude(Renderer* renderer, const Bounds& bounds);
			static void renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, uint16_t autopilotValue, bool autopilotValueEnabled, bool left);
			static void renderGroundSpeed(Renderer* renderer, const Bounds& bounds);
	};
}