#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../theme.h"
#include "page.h"
#include "routing.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MenuItem : public TextElement {
		public:
			MenuItem(std::wstring_view text);
	};

	class TitleMenuItem : public MenuItem {
		public:
			TitleMenuItem(std::wstring_view text);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
	};

	class PageMenuItem : public MenuItem, public SelectableElement {
		public:
			PageMenuItem(std::wstring_view text, const Route* route);

			const Route* getRoute() const;

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event* event) override;

		private:
			const Route* _route;
	};
}