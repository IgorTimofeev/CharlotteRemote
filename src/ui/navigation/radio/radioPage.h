#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/yoba.h"
#include "../../../../lib/YOBA/src/ui.h"

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/rows.h"

using namespace yoba;
using namespace yoba::ui;

namespace pizdanc {
	class RadioPage : public RowsPage {
		public:
			RadioPage() {
				
			}
	};
}