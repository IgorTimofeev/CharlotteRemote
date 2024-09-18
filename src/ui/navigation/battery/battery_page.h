#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class BatteryPage : public Page {
		public:
			BatteryPage() {
				setAlignment(Alignment::Center);


				addChild(&rows);
			}

			Rows rows = Rows();

	};
}