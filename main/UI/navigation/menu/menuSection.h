#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class Menu;

	class MenuSection : public StackLayout {
		public:
			explicit MenuSection();

			Menu* getMenu();

			Text title {};
			WrapLayout wrapLayout {};
	};
}