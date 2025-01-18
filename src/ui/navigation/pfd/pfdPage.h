#pragma once

#include "Arduino.h"
#include "../page.h"
#include "pfd.h"
#include "instruments/instrumentsLayout.h"

namespace pizdanc {
	using namespace yoba;

	class PFDPage : public Page {
		public:
			PFDPage();

		private:
			PFD _pfd {};

			InstrumentsLayout _instrumentsLayout {};
	};
}