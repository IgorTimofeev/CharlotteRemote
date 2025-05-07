#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class Menu;

	class MenuView : public WrapLayout {
		public:
			explicit MenuView();

			Menu* getMenu();
	};
}