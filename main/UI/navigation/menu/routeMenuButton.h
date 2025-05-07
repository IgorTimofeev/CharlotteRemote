#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuButton.h"
#include "../route.h"

namespace pizda {
	using namespace YOBA;

	class RouteMenuButton : public MenuButton {
		public:
			RouteMenuButton(const Image* image, const std::wstring_view& text, const Route* mainRoute, const Route* upperSectionRoute = nullptr);

			const Route* getMainRoute() const;
			const Route* getUpperSectionRoute() const;

		protected:
			void onClick() override;

		private:
			const Route* _mainRoute;
			const Route* _upperSectionRoute;
	};
}