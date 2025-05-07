#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../theme.h"
#include "../../../settings.h"
#include "../page.h"
#include "../route.h"

namespace pizda {
	using namespace YOBA;

	class MenuView;

	class MenuViewButton : public Button {
		public:
			explicit MenuViewButton(const Image* image, const std::wstring_view& text);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			MenuView* getMenuView() const;

		private:
			constexpr static uint8_t _textOffset = 4;

			const Image* _image;
	};
}