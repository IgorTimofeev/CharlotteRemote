#pragma once

#include <string>
#include <format>

#include <YOBA/UI.h>
#include "UI/theme.h"

namespace pizda {
	using namespace YOBA;

	class BatteryIndicator : public Control {
		public:
			BatteryIndicator() {
				setSize(Size(28, 9));
			}
			
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const auto yCenter = bounds.getYCenter();
				const auto tipSize = Size(4, 5);
				
				// +1 because tip overlaps frame
				const uint16_t frameSize = bounds.getWidth() - tipSize.getWidth() + 1;

				const auto frameColor = _voltageMV > 0 ? &Theme::bg4 : &Theme::bad3;
				
				// Frame
				renderer->renderRectangle(
					Bounds(bounds.getX(), bounds.getY(), frameSize, bounds.getHeight()),
					frameColor
				);

				// Tip
				renderer->renderRectangle(
					Bounds(Point(bounds.getX() + frameSize - 1, yCenter - tipSize.getHeight() / 2), tipSize),
					frameColor
				);
				
				// Fill
				if (_voltageMV > 0) {
					const auto fillWidth = static_cast<uint16_t>(static_cast<uint32_t>(frameSize) * _charge / 0xFF);
					
					if (fillWidth > 0) {
						const Color* color;
						
						if (_charge < 0xFF * 1 / 10) {
							color = &Theme::bad1;
						}
						else if (_charge < 0xFF * 2 / 10) {
							color = &Theme::yellow;
						}
						else {
							color = &Theme::good2;
						}
						
						renderer->renderFilledRectangle(
							Bounds(
								bounds.getX(),
								bounds.getY(),
								fillWidth,
								bounds.getHeight()
							),
							color
						);
					}
				}

				// Text
				const auto text =
					_voltageMV > 0
					? std::format(L"{:.1f}", static_cast<float>(_voltageMV) / 1000.f)
					: L"---";

				renderer->renderString(
					Point(
						bounds.getX() + frameSize / 2 - Theme::fontSmall.getWidth(text) / 2,
						bounds.getYCenter() - Theme::fontSmall.getHeight() / 2 + 1
					),
					&Theme::fontSmall,
					_voltageMV > 0 ? &Theme::fg1 : &Theme::bad1,
					text
				);
			}

			uint16_t getVoltage() const {
				return _voltageMV;
			}

			void setVoltage(uint16_t voltageMV) {
				_voltageMV = voltageMV;

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
			uint16_t _voltageMV = 0;
			uint8_t _charge = 0;
	};
}