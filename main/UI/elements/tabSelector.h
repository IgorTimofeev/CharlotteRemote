#pragma once

#include <YOBA/UI.h>
#include <UI/theme.h>

namespace pizda {
	using namespace YOBA;

	class TabSelectorItem : public SelectorItem, public TextElement {
		public:
			TabSelectorItem() {

			}

			explicit TabSelectorItem(std::wstring_view text) : TabSelectorItem() {
				setText(text);
			}

		protected:
			void onRender(Renderer* renderer) override {
				const auto& bounds = getBounds();

				if (isActive()) {
					renderer->renderFilledRectangle(
						bounds,
						Theme::cornerRadius,
						&Theme::fg1
					);
				}

				renderer->renderString(
					Point(
						bounds.getXCenter() - Theme::fontNormal.getWidth(getText()) / 2,
						bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
					),
					&Theme::fontNormal,
					isActive() ? &Theme::bg1 : &Theme::fg4,
					getText()
				);
			}
	};

	class TabSelector : public Selector {
		public:
			TabSelector() {
				setHeight(Theme::elementHeight);

				applyPageStyle();

				backgroundRectangle.setCornerRadius(Theme::cornerRadius);
				*this += &backgroundRectangle;

				itemsLayout.setOrientation(Orientation::horizontal);
				itemsLayout.setSpacing(1);
				*this += &itemsLayout;
				setItemsLayout(&itemsLayout);
			}

			Rectangle backgroundRectangle {};
			RelativeStackLayout itemsLayout {};

			void applyPageStyle() {
				backgroundRectangle.setFillColor(&Theme::bg2);
				backgroundRectangle.setBorderColor(&Theme::bg3);
			}

			void applyDialogStyle() {
				backgroundRectangle.setFillColor(&Theme::bg1);
				backgroundRectangle.setBorderColor(&Theme::bg3);
			}
	};
}