#pragma once

#include "../../rc_application.h"
#include "../../theme.h"

using namespace yoba;
using namespace pizdanc;

class PFD : public Element {
	public:
		PFD();

		void renderAutopilotValue(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t unitPixels, float currentValue, float autopilotValue, bool left);

		void renderCurrentValue(ScreenBuffer* screenBuffer, const Bounds& bounds, int32_t centerY, float currentValue, bool left);

		void renderTrendArrow(ScreenBuffer* screenBuffer, int32_t x, int32_t y, uint8_t unitStep, uint16_t unitPixels, float value);

		void renderSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds);

		void renderHorizon(ScreenBuffer* screenBuffer, const Bounds& bounds);

		void renderAltitude(ScreenBuffer* screenBuffer, const Bounds& bounds);

		void renderVerticalSpeed(ScreenBuffer* screenBuffer, const Bounds& bounds);

		void renderMiniPanel(ScreenBuffer* screenBuffer, const Bounds &bounds, const Color* bg, const Color* fg, char* buffer);

		void renderPressure(ScreenBuffer* screenBuffer, const Bounds& bounds);

		void renderAutopilotSpeed(ScreenBuffer* screenBuffer, const Bounds &bounds);

		void renderAutopilotAltitude(ScreenBuffer* screenBuffer, const Bounds &bounds);

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

		const uint16_t speedBarSize = 3;

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
		const uint8_t autopilotIndicatorWidth = 5;
		const uint8_t autopilotIndicatorHeight = 15;
		const uint8_t autopilotIndicatorHeightHalf = autopilotIndicatorHeight / 2;

		const uint8_t autopilotIndicatorTriangleMargin = 3;
		const uint8_t autopilotIndicatorTriangleWidth = 4;
		const uint8_t autopilotIndicatorTriangleHeight = autopilotIndicatorHeight - autopilotIndicatorTriangleMargin * 2;

		const uint8_t autopilotIndicatorRectangleWidth = autopilotIndicatorWidth - autopilotIndicatorTriangleWidth;
};