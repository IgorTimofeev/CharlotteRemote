#pragma once

#include <format>
#include <YOBA/UI.h>
#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;

	class BatteryIndicator : public Element {
		public:
			void onRender(Renderer* renderer) override {
				const auto& bounds = getBounds();

				const auto yCenter = bounds.getYCenter();
				const auto tipSize = Size(4, 6);
				const auto tipPosition = Point(bounds.getX2() - tipSize.getWidth(), yCenter - tipSize.getHeight() / 2);
				const uint16_t borderWidth = bounds.getWidth() - tipSize.getWidth();

				// Border
				renderer->renderRectangle(Bounds(bounds.getX(), bounds.getY(), borderWidth, bounds.getHeight()), &Theme::bg4);

				// Tip
				renderer->renderRectangle(Bounds(tipPosition, tipSize), &Theme::bg4);

				// Fill
				const auto fillWidth = static_cast<uint16_t>(static_cast<uint32_t>(borderWidth - 2) * _charge / 0xFF);

				if (fillWidth > 0) {
					const Color* color;

					if (_charge < 0xFF * 1 / 4) {
						color = &Theme::bad2;
					}
					else if (_charge < 0xFF * 3 / 4) {
						color = &Theme::yellow;
					}
					else {
						color = &Theme::good2;
					}

					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + 1,
							bounds.getY() + 1,
							fillWidth,
							bounds.getHeight() - 2
						),
						color
					);
				}

				// Text
				const auto text = std::format(L"{:.1f}v", static_cast<float>(_voltage) / 1000.f);

				renderer->renderString(
					Point(
						bounds.getX() + borderWidth / 2 - Theme::fontSmall.getWidth(text) / 2,
						bounds.getYCenter() - Theme::fontSmall.getHeight() / 2
					),
					&Theme::fontSmall,
					&Theme::fg1,
					text
				);
			}

			uint16_t getValue() const {
				return _voltage;
			}

			void setVoltage(uint16_t value) {
				_voltage = value;

				invalidate();
			}

			uint8_t getCharge() const {
				return _charge;
			}

			void setCharge(uint8_t charge) {
				_charge = charge;

				invalidate();
			}

		private:
			uint16_t _voltage = 0;
			uint8_t _charge = 0;
	};
}