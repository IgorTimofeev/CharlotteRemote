#pragma once

#include "Arduino.h"

#include "yoba/elements/slider.h"
#include "yoba/elements/text.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

using namespace yoba;

namespace ui {
	class EnginePage : public Page {
		public:
			EnginePage() {
				setAlignment(Alignment::center);


				addChild(&rows);
			}

			Rows rows = Rows();
	};
}