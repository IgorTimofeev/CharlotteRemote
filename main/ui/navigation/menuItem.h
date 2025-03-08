#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../theme.h"
#include "page.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MenuItem : public SelectorItem, public TextElement {
		public:
			MenuItem(std::wstring_view text);
	};

	class TitleMenuItem : public MenuItem {
		public:
			TitleMenuItem(std::wstring_view text);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
	};

	class PageMenuItem : public MenuItem {
		public:
			PageMenuItem(std::wstring_view text, const std::function<Page*()>& pageBuilder);

			const std::function<Page*()>& getPageBuilder() const;

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			std::function<Page*()> _pageBuilder;
	};
}