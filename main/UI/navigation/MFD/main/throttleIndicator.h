#pragma once

#include <format>
#include <YOBA/UI.h>
#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;

	class ThrottleIndicator : public Element {
		public:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const uint8_t textOffset = 3;
				const uint16_t frameHeight = bounds.getHeight() - Theme::fontSmall.getHeight() - textOffset;
				const auto frameY2 = bounds.getY() + frameHeight - 1;

				// Frame
				renderer->renderRectangle(
					Bounds(
						bounds.getX(),
						bounds.getY(),
						bounds.getWidth(),
						frameHeight
					),
					&Theme::bg5
				);

				// Aircraft value fill
				const auto aircraftValueFillHeight = (uint16_t) (frameHeight * _aircraftValue / 0xFF);

				if (aircraftValueFillHeight > 0) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							frameY2 - aircraftValueFillHeight,
							bounds.getWidth(),
							aircraftValueFillHeight
						),
						&Theme::fg1
					);
				}

				// Remote value line
				const uint8_t remoteValueLineOffsetX = 2;
				const auto remoteValueLineHeight = (uint16_t) (frameHeight * _remoteValue / 0xFF);

				renderer->renderHorizontalLine(
					Point(
						bounds.getX() - remoteValueLineOffsetX,
						frameY2 - remoteValueLineHeight
					),
					remoteValueLineOffsetX + bounds.getWidth() + remoteValueLineOffsetX,
					&Theme::green
				);

				// Aircraft value text
				const auto text = std::to_wstring((int32_t) (_aircraftValue * 100 / 0xFF));

				renderer->renderString(
					Point(
						bounds.getX() + bounds.getWidth() / 2 - Theme::fontSmall.getWidth(text) / 2 + 1,
						bounds.getY() + frameHeight + textOffset
					),
					&Theme::fontSmall,
					&Theme::green,
					text
				);
			}

			uint8_t getRemoteValue() const {
				return _remoteValue;
			}

			void setRemoteValue(uint8_t value) {
				_remoteValue = value;

				invalidate();
			}

			uint8_t getAircraftValue() const {
				return _aircraftValue;
			}

			void setAircraftValue(uint8_t aircraftValue) {
				_aircraftValue = aircraftValue;
			}

		private:
			uint8_t _remoteValue = 0;
			uint8_t _aircraftValue = 0;
	};
}