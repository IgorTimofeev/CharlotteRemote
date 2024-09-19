#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/rows.h"

using namespace yoba;

namespace pizdanc {
	class RadioPage : public Page {
		public:
			RadioPage() {



				addChild(&rows);
			}

			Rows rows = Rows();
	};
}