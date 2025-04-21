#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "../../theme.h"
#include "../../../settings.h"

#include "menuView.h"
#include "menuButton.h"
#include "../routes.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MenuOverlayBackground : public DitheredRectangle {
		public:
			MenuOverlayBackground();

		protected:
			void onEvent(Event* event) override;
	};

	class Menu : public RelativeStackLayout {
		public:
			explicit Menu(const Route* route);
			~Menu();

			void setView(const Route* value);

		private:
			MenuOverlayBackground _overlayBackground = {};

			Layout _slideLayout = {};
			Rectangle _slideBackground = {};

			const Route* _route = nullptr;
			Element* _view = nullptr;

	};
}