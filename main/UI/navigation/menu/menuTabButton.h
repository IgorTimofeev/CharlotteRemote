#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/theme.h"
#include "settings/settings.h"
#include "UI/navigation/page.h"
#include "UI/navigation/route.h"

namespace pizda {
	using namespace YOBA;

	class Menu;
	class MenuView;

	class MenuTabButton : public Button {
		public:
			MenuTabButton(std::wstring_view text, const Route* route);

			const Route* getRoute() const;

		protected:
			void onClick() override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			const Route* _route;
	};
}