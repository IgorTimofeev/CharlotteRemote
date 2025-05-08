#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuViewButton.h"
#include "../route.h"

namespace pizda {
	using namespace YOBA;

	class PageMenuViewButton : public MenuViewButton {
		public:
			PageMenuViewButton(const Image* image, std::wstring_view text, const Route* route);

			const Route* getRoute() const;

		protected:
			void onClick() override;

		private:
			const Route* _route;
	};
}