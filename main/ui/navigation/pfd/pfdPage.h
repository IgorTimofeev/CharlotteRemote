#pragma once

#include "../page.h"
#include "pfd.h"
#include "instruments/instrumentsLayout.h"

namespace pizda {
	using namespace yoba;

	class PFDPage : public Page {
		public:
			PFDPage();

			void setup() override;

		private:
			PFD _pfd;
			InstrumentsLayout _instrumentsLayout;
	};
}