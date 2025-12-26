#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/theme.h"
#include <settings/settings.h>
#include <UI/navigation/page.h>
#include <UI/navigation/route.h>

namespace pizda {
	using namespace YOBA;

	class MenuView;

	class MenuViewButton : public Button {
		public:
			MenuViewButton(const Image* image, std::wstring_view text);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			MenuView* getMenuView() const;

		private:
			constexpr static uint8_t _textOffset = 4;

			const Image* _image;
	};
}