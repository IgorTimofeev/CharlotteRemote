#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../theme.h"
#include "../../../settings.h"
#include "../page.h"
#include "../route.h"

namespace pizda {
	using namespace YOBA;

	class Menu;
	class MenuView;

	class MenuTabButton : public Button {
		public:
			MenuTabButton(const std::wstring_view& text, const Route* route);

			const Route* getRoute() const;

		protected:
			void onClick() override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			const Route* _route;
	};
}