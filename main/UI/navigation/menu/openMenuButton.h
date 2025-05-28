#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class OpenMenuButton : public Button {
		public:
			void applyBottomStyle();
			void applyBottomRightStyle();

		protected:
			void onClick() override;

		private:
			constexpr static uint8_t _roundStyleSize = 36;

			void applyRoundStyle();
	};
}