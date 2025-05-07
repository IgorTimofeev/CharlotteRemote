#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "../route.h"

namespace pizda {
	using namespace YOBA;

	class Menu;

	class MenuView : public WrapLayout {
		public:
			explicit MenuView();

			Menu* getMenu();

			virtual const Route* getRoute() = 0;
	};
}