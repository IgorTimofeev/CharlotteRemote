#pragma once

#include "Arduino.h"

#include "ui/text.h"
#include "ui/slider.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class ControlsPage : public Page {
		public:
			ControlsPage() {
				setAlignment(Alignment::Center);


				addChild(&rows);
			}

			Rows rows = Rows();

	};
}