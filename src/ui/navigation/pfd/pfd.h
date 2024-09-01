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

		void speedRenderBar(Screen& screen, float& speed, uint16_t& centerY, int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color* color) const;

		void renderTrendArrow(Screen &screen, int32_t x, int32_t y, uint16_t unitPixels, float value);

		void speedRender(Screen& screen, const Bounds& bounds);

		void horizonRender(Screen& screen, const Bounds& bounds);

		void altitudeRender(Screen& screen, const Bounds& bounds);

		void verticalSpeedRender(Screen& screen, const Bounds& bounds);

		void onRender(Screen& screen) override;

	private:
		const uint16_t lineSizeBig = 5;
		const uint16_t lineSizeSmall = 2;

		const uint16_t currentValueHeight = 20;
		const uint8_t currentValueTriangleWidth = 8;

		// Speed
		const uint8_t speedWidth = 32;
		const uint8_t speedUnitPixels = 8;
		const uint8_t speedStepUnits = 1;
		const uint8_t speedStepUnitsBig = 5;

		const uint16_t speedBarWidth = 4;

		const uint16_t speedStallMin = 0;
		const uint16_t speedStallMax = 10;

		const uint16_t speedLandingMin = 10;
		const uint16_t speedLandingMax = 15;

		const uint16_t speedTakeOffMin = 15;
		const uint16_t speedTakeOffMax = 30;

		const uint16_t speedCruiseMin = 30;
		const uint16_t speedCruiseMax = 80;

		const uint16_t speedOverMin = 80;
		const uint16_t speedOverMax = 100;

		const uint16_t speedStressMin = 100;
		const uint16_t speedStressMax = 0xFFFF;

		// Altitude
		const uint8_t altitudeWidth = 32;
		const uint8_t altitudeStepUnits = 1;
		const uint8_t altitudeStepUnitsBig = 5;
		const uint8_t altitudeUnitPixels = 8;

		// Vertical speed
		const uint8_t verticalSpeedWidth = 18;
		const uint16_t verticalSpeedStepUnits = 250;
		const uint16_t verticalSpeedStepUnitsBig = 1000;
		const uint16_t verticalSpeedUnitPixels = 12;
		const uint16_t verticalSpeedRightUnitPixels = 5;
};