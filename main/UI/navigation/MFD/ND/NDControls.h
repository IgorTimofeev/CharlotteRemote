#pragma once

#include "ND.h"

#include <esp_log.h>

#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;

	class NDControls : public Layout {
		public:
			NDControls() {
//				setHeight(140);

				*this += &_ND;

				// Right rows
				_rightRows.setSpacing(10);
				_rightRows.setAlignment(Alignment::end, Alignment::start);
				_rightRows.setMargin(Margin(0, 5, 5, 0));
				*this += &_rightRows;

				// Center button
				addGovnoButton(&_latLongButton, L"L", [this]() {
					_ND.resetCameraOffsetLatLon();
				});

				addGovnoButton(&_altButton, L"A", [this]() {
					_ND.setCameraOffset(GeographicCoordinates(
						_ND.getCameraOffset().getLatitude(),
						_ND.getCameraOffset().getLongitude(),
						ND::cameraAltitudeMinimum
					));
				});

//				// Slider
//				Theme::apply(&_slider);
//
//				_slider.setMargin(Margin(10, 0, 10, 10));
//				_slider.setVerticalAlignment(Alignment::end);
//
//				_slider.valueChanged += [this]() {
//					auto fov = (float) _slider.getValue() / (float) 0xFFFF * toRadians(180.f);
//
//					ESP_LOGI("ND", "FOV: %f deg", toDegrees(fov));
//
//					_ND.getCamera().setFOV(fov);
//				};
//
//				*this += &_slider;
			}

		private:
			ND _ND {};

			StackLayout _rightRows {};
			Button _latLongButton {};
			Button _altButton {};

//			Slider _slider {};

			void addGovnoButton(Button* button, const std::wstring_view& text, const std::function<void()>& onClick) {
				Theme::apply(button);

				button->setSize(Size(20));

				button->setDefaultBackgroundColor(&Theme::bg2);
				button->setDefaultTextColor(&Theme::fg5);

				button->setPressedBackgroundColor(&Theme::fg1);
				button->setPressedTextColor(&Theme::bg1);

				button->setFont(&Theme::fontSmall);
				button->setText(text);

				button->click += onClick;

				_rightRows += button;
			}
	};
}