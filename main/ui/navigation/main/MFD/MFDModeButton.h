#pragma once

#include "../../../../../components/yoba/src/main.h"
#include "../../../../../components/yoba/src/ui.h"
#include "../../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MFDModeButton : public SelectableElement, public TextElement {
		public:
			explicit MFDModeButton(const std::wstring_view& text) {
				setText(text);
			}

			Callback<Event*> gotEvent {};

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				renderer->renderFilledRectangle(bounds, isSelected() ? &Theme::bg3 : &Theme::bg2);

				if (isSelected())
					renderer->renderHorizontalLine(bounds.getBottomLeft(), bounds.getWidth(), &Theme::fg1);

				renderer->renderString(
					Point(
						bounds.getXCenter() - Theme::fontNormal.getWidth(getText()) / 2,
						bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
					),
					&Theme::fontNormal,
					isSelected() ? &Theme::fg1 : &Theme::fg6,
					getText()
				);
			}

		protected:
			void onEvent(Event* event) override {
				Element::onEvent(event);

				gotEvent(event);
			}
	};
}