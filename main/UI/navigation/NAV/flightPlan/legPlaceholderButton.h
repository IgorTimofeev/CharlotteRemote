#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class LegPlaceholderButton : public Button {
		public:
			LegPlaceholderButton();

		protected:
			void onClick() override;
	};
}