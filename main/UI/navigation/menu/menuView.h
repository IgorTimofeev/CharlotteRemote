#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <UI/navigation/route.h>

namespace pizda {
	using namespace YOBA;

	class Menu;

	class MenuView : public WrapLayout {
		public:
			explicit MenuView();

			virtual void setup();

			Menu* getMenu() const;

			virtual const Route* getRoute() = 0;
	};
}