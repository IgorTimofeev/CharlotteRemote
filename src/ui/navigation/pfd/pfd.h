#pragma once

#include "yoba/elements/element.h"
#include "yoba/screen.h"
#include "../../rc_application.h"
#include "../../theme.h"

using namespace yoba;
using namespace pizdanc;

class PFD : public Element {
	public:
		PFD();

		void renderTrendArrow(Screen &screen, int32_t x, int32_t y, uint8_t unitStep, uint16_t unitPixels, float value);

		void speedRender(Screen& screen, const Bounds& bounds);

		void horizonRender(Screen& screen, const Bounds& bounds);

		void altitudeRender(Screen& screen, const Bounds& bounds);

		void verticalSpeedRender(Screen& screen, const Bounds& bounds);

		void onRender(Screen& screen) override;

	private:
		const uint16_t lineSizeBig = 5;
		const uint16_t lineSizeSmall = 2;

		const uint16_t currentValueHeight = 20;
		const uint8_t currentValueTriangleSize = 8;

		// Speed
		const uint8_t speedWidth = 32;
		const uint8_t speedUnitPixels = 8;
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
		const uint8_t pressureHeight = 16;

		// Vertical speed
		const uint8_t verticalSpeedWidth = 18;
		const uint16_t verticalSpeedStepUnits = 250;
		const uint16_t verticalSpeedStepUnitsBig = 1000;
		const uint16_t verticalSpeedUnitPixels = 13;
		const uint16_t verticalSpeedRightUnitPixels = 5;
};