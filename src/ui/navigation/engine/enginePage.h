#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/yoba.h"
#include "../../../../lib/YOBA/src/ui.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"

using namespace yoba;
using namespace yoba::ui;

namespace pizdanc {
	class EnginePage : public RowsPage {
		public:
			EnginePage() {

			}
	};
}