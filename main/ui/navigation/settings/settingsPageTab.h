#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../../theme.h"
#include "../page.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class SettingsPageTab : public SelectorItem, public TextElement {
		public:
			SettingsPageTab(const std::wstring_view& text, const std::function<Page*()>& pageBuilder) : _pageBuilder(pageBuilder) {
				setText(text);
			}

			const std::function<Page*()>& getPageBuilder() const {
				return _pageBuilder;
			}

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				if (isSelected()) {
					// Background
					renderer->renderFilledRectangle(bounds, &Theme::bg3);

					// Line
					renderer->renderVerticalLine(
						bounds.getTopLeft(),
						bounds.getHeight(),
						&Theme::accent1
					);
				}

				// Text
				renderer->renderString(
					Point(
						bounds.getXCenter() - Theme::fontNormal.getWidth(getText()) / 2,
						bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
					),
					&Theme::fontNormal,
					isSelected() ? &Theme::fg1 : &Theme::fg4,
					getText()
				);
			}

		private:
			std::function<Page*()> _pageBuilder;
	};
}