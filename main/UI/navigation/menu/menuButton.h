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

	class MenuButton : public Button {
		public:
			explicit MenuButton(const Image* image, const std::wstring_view& text);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			MenuView* getMenuView();

		private:
			constexpr static const uint8_t _textOffset = 4;

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