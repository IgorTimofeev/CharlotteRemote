#pragma once

#include "Arduino.h"

#include "yoba/elements/slider.h"
#include "yoba/elements/text.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"
#include "pfd.h"

using namespace yoba;

namespace ui {
	class PFDPage : public Page {
		public:
			PFDPage() {
				_pfd.setSize(Size(320, 240));
				addChild(&_pfd);
			}

		private:
			PFD _pfd = PFD();
	};
}