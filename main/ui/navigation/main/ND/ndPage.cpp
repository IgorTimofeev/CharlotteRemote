#include <esp_log.h>
#include "ndPage.h"

namespace pizda {
	NDPage::NDPage() {
		*this += &_nd;

		// FOV
		Theme::apply(&_FOVSlider);

		_FOVSlider.valueChanged += [this]() {
			auto fov = (float) _FOVSlider.getValue() / (float) 0xFFFF * toRadians(180.f);

			ESP_LOGI("ND", "FOV deg: %f", toDegrees(fov));

			_nd.getCamera().setFOV(fov);
		};

		_slidersLayout += &_FOVSlider;

		// CLip
		Theme::apply(&_nearSlider);

		_nearSlider.valueChanged += [this]() {
			_nd.getCamera().setNearPlaneDistance(0.1f + _nearSlider.getValue() * 100 / 0xFFFF);

			ESP_LOGI("ND", "Clip: %d", _nearSlider.getValue() / 0xFFFF);
		};

		_slidersLayout += &_nearSlider;

		_slidersLayout.setSpacing(5);
		_slidersLayout.setVerticalAlignment(Alignment::end);
		_slidersLayout.setMargin(Margin(10, 0, 10, 10));
		*this += &_slidersLayout;

		// Menu button
		_menuButton.setAlignment(Alignment::start, Alignment::start);
		_menuButton.setMargin(Margin(10, 10, 0, 0));
		*this += &_menuButton;
	}
}