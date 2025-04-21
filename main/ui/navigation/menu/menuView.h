#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Menu;

	class MenuView : public StackLayout {
		public:
			explicit MenuView();

			Menu* getMenu();

		protected:
			Text title {};
			WrapLayout wrapLayout {};
	};
}