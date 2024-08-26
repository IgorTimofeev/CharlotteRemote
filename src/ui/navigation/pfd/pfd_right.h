#pragma once

#include "yoba/elements/element.h"
#include "yoba/screen.h"
#include "../../rc_application.h"
#include "../../theme.h"

using namespace yoba;
using namespace pizdanc;

class PFDRight : public Element {
	public:
		PFDRight();

		void onRender(Screen &screen) override;

	private:
		const uint16_t lineSizeBig = 8;
		const uint16_t lineSizeSmall = 5;
};