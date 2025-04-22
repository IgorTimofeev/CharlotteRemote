#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

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