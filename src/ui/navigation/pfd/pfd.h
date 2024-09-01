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

		void onRender(Screen& screen) override;

	private:
		const uint8_t sidePanelWidth = 34;

		const uint16_t lineSizeBig = 8;
		const uint16_t lineSizeSmall = 5;

		const uint8_t speedUnitPixels = 8;
		const uint8_t speedStepUnits = 1;
		const uint8_t speedStepUnitsBig = 5;

		const uint16_t speedBarWidth = 4;

		const uint16_t speedStallMin = 0;
		const uint16_t speedStallMax = 5;

		const uint16_t speedLandingMin = 5;
		const uint16_t speedLandingMax = 8;

		const uint16_t speedTakeOffMin = 8;
		const uint16_t speedTakeOffMax = 12;

		const uint16_t speedCruiseMin = 12;
		const uint16_t speedCruiseMax = 30;

		const uint16_t speedOverMin = 80;
		const uint16_t speedOverMax = 100;

		const uint16_t speedStressMin = 100;
		const uint16_t speedStressMax = 0xFFFF;
};