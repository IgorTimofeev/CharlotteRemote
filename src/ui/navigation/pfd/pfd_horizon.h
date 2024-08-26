#pragma once

#include "yoba/elements/element.h"
#include "yoba/screen.h"
#include "../../rc_application.h"
#include "../../theme.h"

using namespace yoba;
using namespace pizdanc;

class PFDHorizon : public Element {
	public:
		PFDHorizon();

		void onRender(Screen &screen) override;
};