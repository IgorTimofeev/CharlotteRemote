#pragma once

#include <functional>
#include <YOBA/UI.h>

#include "UI/theme.h"

namespace pizda {
	using namespace YOBA;

	class Referencer : public Layout {
		public:
			constexpr static uint8_t arrowOffset = 10;
			constexpr static uint8_t arrowWidth = 5;
			constexpr static uint8_t arrowHeightDiv2 = 5;

			void setDefaultMargin(Element* element, const Margin& margin) const {
				element->setMargin(Margin {
					margin.getLeft(),
					margin.getTop(),
					margin.getRight() + arrowWidth + arrowOffset,
					margin.getBottom()
				});
			}

			void setOnClick(const std::function<void()>& onClick) {
				_onClick = onClick;
			}

		protected:
			virtual void onClick() {

			}

			void onEventBeforeChildren(Event* event) override {
				Layout::onEventBeforeChildren(event);

				if (event->getTypeID() == PointerDownEvent::typeID) {
					setFocused(true);

					event->setHandled(true);
				}
				else if (event->getTypeID() == PointerUpEvent::typeID) {
					setFocused(false);

					onClick();

					if (_onClick)
						_onClick();

					event->setHandled(true);
				}
			}

			void onPointerOverChanged() override {
				if (!isPointerOver())
					setFocused(false);
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				renderer->renderFilledRectangle(bounds, Theme::cornerRadius, isFocused() ? &Theme::bg4 : &Theme::bg3);
				renderer->renderRectangle(bounds, Theme::cornerRadius, isFocused() ? &Theme::bg5 : &Theme::bg4);

				const Point arrowPos {
					bounds.getX2() - arrowOffset,
					bounds.getYCenter()
				};

				renderer->renderLine(
					arrowPos,
					Point {
						arrowPos.getX() - arrowWidth,
						arrowPos.getY() - arrowHeightDiv2
					},
					&Theme::fg5
				);

				renderer->renderLine(
					arrowPos,
					Point {
						arrowPos.getX() - arrowWidth,
						arrowPos.getY() + arrowHeightDiv2
					},
					&Theme::fg5
				);

				Layout::onRender(renderer, bounds);
			}

		private:
			std::function<void()> _onClick = nullptr;
	};
}