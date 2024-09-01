#pragma once

#include "yoba/elements/element.h"
#include "yoba/screen.h"
#include "../../rc_application.h"
#include "../../theme.h"

using namespace yoba;
using namespace pizdanc;

class PFDLeft : public Element {
	public:
		PFDLeft();

		void renderSpeedBar(Screen& screen, float& speed, uint16_t& centerY, int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color* color) const;

		void onRender(Screen& screen) override;

	private:
		const uint8_t leftWidth = 34;
		const uint8_t rightWidth = 34;

		const uint8_t unitPixels = 10;
		const uint8_t stepUnits = 1;
		const uint8_t stepUnitsBig = 5;

		const uint16_t lineSizeBig = 8;
		const uint16_t lineSizeSmall = 5;

		const uint16_t speedBarWidth = 4;

		const uint16_t stallSpeedMin = 0;
		const uint16_t stallSpeedMax = 5;

		const uint16_t landingSpeedMin = 5;
		const uint16_t landingSpeedMax = 8;

		const uint16_t takeOffSpeedMin = 8;
		const uint16_t takeOffSpeedMax = 12;

		const uint16_t cruiseSpeedMin = 12;
		const uint16_t cruiseSpeedMax = 30;

		const uint16_t overSpeedMin = 30;
		const uint16_t overSpeedMax = 100;
};