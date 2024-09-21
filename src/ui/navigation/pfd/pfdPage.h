#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"
#include "pfd.h"

using namespace yoba;

namespace pizdanc {
	class PFDPage : public Page {
		public:
			PFDPage() {
				*this += &_pfd;
			}

		private:
			PFD _pfd = PFD();
	};
}