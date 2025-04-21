#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "../../theme.h"
#include "../../../settings.h"
#include "../page.h"
#include "../route.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class Menu;
	class MenuView;

	class MenuButton : public Button {
		public:
			explicit MenuButton(const Image* image, const std::wstring_view& text);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			MenuView* getMenuView();

		private:
			const Image* _image;
	};

	class ViewMenuButton : public MenuButton {
		public:
			ViewMenuButton(const Image* image, const std::wstring_view& text, const Route* route);

		protected:
			void onClick() override;

		private:
			const Route* _route;
	};
}