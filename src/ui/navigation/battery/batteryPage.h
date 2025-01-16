#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/yoba.h"
#include "../../../../lib/YOBA/src/ui.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class BatteryPage : public RowsPage {
		public:
			BatteryPage() {

			}
	};
}