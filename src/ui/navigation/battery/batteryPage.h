#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"

using namespace yoba;

namespace pizdanc {
	class BatteryPage : public RowsPage {
		public:
			BatteryPage() {

			}
	};
}