#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../theme.h"
#include "page.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class Tab : public SelectorItem, public TextElement {
		public:
			Tab(const std::wstring_view& text, const std::function<Page*()>& pageBuilder) : _pageBuilder(pageBuilder) {
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

//					// Line
//					static const uint8_t lineHorizontalOffset = 9;
//					static const uint8_t lineVerticalOffset = 2;
//
//					renderer->renderHorizontalLine(
//						Point(
//							bounds.getX() + lineHorizontalOffset,
//							bounds.getY2() - lineVerticalOffset
//						),
//						bounds.getWidth() - lineHorizontalOffset * 2,
//						&Theme::green
//					);

					renderer->renderHorizontalLine(
						bounds.getBottomLeft(),
						bounds.getWidth(),
						&Theme::fg1
					);
				}

				// Text
				static const uint8_t textOffset = 1;

				renderer->renderString(
					Point(
						bounds.getXCenter() - Theme::fontNormal.getWidth(getText()) / 2,
						bounds.getYCenter() - Theme::fontNormal.getHeight() / 2 - textOffset
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