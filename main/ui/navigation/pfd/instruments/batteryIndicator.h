#pragma once

#include <sstream>
#include <iomanip>
#include "../../../components/yoba/src/ui.h"
#include "../../../theme.h"

namespace pizda {
	using namespace yoba;

	class BatteryIndicator : public Element {
		public:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const int32_t yCenter = bounds.getYCenter();

				const auto tipSize = Size(4, 6);
				const auto tipPosition = Point(bounds.getX2() - tipSize.getWidth(), yCenter - tipSize.getHeight() / 2);
				const uint16_t borderWidth = bounds.getWidth() - tipSize.getWidth();

				// Border
				renderer->renderRectangle(Bounds(bounds.getX(), bounds.getY(), borderWidth, bounds.getHeight()), &Theme::bg4);

				// Tip
				renderer->renderRectangle(Bounds(tipPosition, tipSize), &Theme::bg4);

				// Fill
				const auto fillWidth = (uint16_t) ((uint32_t) (borderWidth - 2) * _charge / 0xFF);

				if (fillWidth > 0) {
					const Color* color;

					if (_charge < 0xFF * 1 / 3) {
						color = &Theme::bad2;
					}
					else if (_charge < 0xFF * 2 / 3) {
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
				std::wstringstream stream;
				stream.str(std::wstring());
				stream << yoba::round(_voltage / 1000.f, 1);
				stream << L"v";

				const auto text = stream.str();

				// Crash, wtf
//				const auto text = std::format(L"{}v", yoba::round(_voltage / 1000.f, 1));

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
			}

		private:
			uint16_t _voltage = 0;
			uint8_t _charge = 0;
	};
}