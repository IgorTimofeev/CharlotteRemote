#pragma once

#include "ND.h"

#include <esp_log.h>

#include "../../../../theme.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class NDControls : public Layout {
		public:
			NDControls() {
				setHeight(140);

				*this += &_ND;

				// Right rows
				_rightRows.setSpacing(10);
				_rightRows.setAlignment(Alignment::end, Alignment::start);
				_rightRows.setMargin(Margin(0, 10, 10, 0));
				*this += &_rightRows;

				// Center button
				Theme::apply(&_centerButton);

				_centerButton.setSize(Size(20));

				_centerButton.setDefaultBackgroundColor(&Theme::bg2);
				_centerButton.setPressedBackgroundColor(&Theme::fg1);

				_centerButton.setDefaultTextColor(&Theme::fg1);
				_centerButton.setPressedTextColor(&Theme::bg2);

				_centerButton.setText(L"C");

				_centerButton.click += [this]() {
					_ND.setCameraOffset(Vector3F(0, 0, _ND.getCameraOffset().getZ()));
				};

				_rightRows += &_centerButton;


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

			ND _ND {};

			StackLayout _rightRows {};
			Button _centerButton {};

//			Slider _slider {};
	};
}