#pragma once

#include "Arduino.h"

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "ui/navigation/page.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class BatteryPage : public RowsPage {
		public:
			BatteryPage() {

			}
	};
}