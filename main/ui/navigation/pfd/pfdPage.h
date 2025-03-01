#pragma once

#include "../page.h"
#include "pfd.h"
#include "instruments/instrumentsLayout.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class PFDPage : public Page {
		public:
			PFDPage();

		protected:
			void onTick() override;

		private:
			PFD _pfd;
			InstrumentsLayout _instrumentsLayout;
	};
}