#pragma once

#include "src/main.h"
#include "src/ui.h"
#include "../../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class PFD : public Element {
		public:
			PFD();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

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

			// Altitude
			constexpr static const uint8_t altitudeWidth = 30;
			constexpr static const uint8_t altitudeStepUnits = 20;
			constexpr static const uint8_t altitudeStepUnitsBig = 100;
			constexpr static const uint8_t altitudeUnitPixels = 8;

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
			constexpr static const uint8_t pitchOverlayAngleStep = 5;
			constexpr static const uint8_t pitchOverlayLineSmall = 5;
			constexpr static const uint8_t pitchOverlayLineBig = 10;
			constexpr static const uint8_t pitchOverlayLineMiddle = 15;
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
			constexpr static const uint8_t turnCoordinatorOverlaySlipAndSkidIndicatorMaxValuePixels = 45;

			// Aircraft symbol
			constexpr static const uint8_t aircraftSymbolWidth = 30;
			constexpr static const uint8_t aircraftSymbolThickness = 2;
			constexpr static const uint8_t aircraftSymbolCenterOffset = 20;

			// Flight path vector
			constexpr static const uint8_t flightPathVectorRadius = 4;
			constexpr static const uint8_t flightPathVectorLineLength = 6;
			constexpr static const uint8_t flightPathVectorLineThickness = 2;
			constexpr static const uint8_t flightPathVectorVisibilityGroundSpeed = 10;

			float _horizontalFOV = toRadians(120);
			float _verticalFOV = toRadians(80);

			static void renderAutopilotValueIndicator(Renderer* renderer, const Point& point, bool left) ;

			static void renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, uint16_t autopilotValue, bool left) ;

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

			static void renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, uint16_t autopilotValue, bool left) ;

			static void renderPitchOverlay(
				Renderer* renderer,
				const Bounds& bounds,
				float unfoldedFOVHeight,
				const Point& horizonLeft,
				const Point& horizonRight,

				const Vector2F& horizonVec,
				const Vector2F& horizonVecNorm,
				const Vector2F& horizonVecPerp,
				const Vector2F& horizonVecCenter
			);

			static void renderSyntheticVisionBackground(Renderer* renderer, const Bounds& bounds, const Point& horizonLeft, const Point& horizonRight);

			static void renderTurnCoordinatorOverlay(Renderer* renderer, const Bounds& bounds, float roll);

			static void renderYawOverlay(Renderer* renderer, const Bounds& bounds, float yaw);

			void renderAircraftAndFPVOverlay(
				Renderer* renderer,
				const Point& center,
				uint16_t unfoldedFOVWidth,
				uint16_t unfoldedFOVHeight,
				const Vector2F& horizonVecCenter
			) const;

			void renderGroundSpeed(Renderer* renderer, const Bounds& bounds);

			void renderWind(Renderer* renderer, const Point& bottomLeft) const;
	};
}