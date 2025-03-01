#include <esp_log.h>
#include "ndPage.h"

namespace pizda {
	NDPage::NDPage() {
		*this += &_nd;

		// Projection
		Theme::apply(&_projectionSlider);

		_projectionSlider.valueChanged += [this]() {
			_nd.getCamera().setProjectionPlaneDistance(0.1f + _projectionSlider.getValue() * 1000 / 0xFFFF);

			ESP_LOGI("ND", "Proj: %d", _projectionSlider.getValue() / 0xFFFF);
		};

		_slidersLayout += &_projectionSlider;

		// CLip
		Theme::apply(&_clipSlider);

		_clipSlider.valueChanged += [this]() {
			_nd.getCamera().setNearPlaneDistance(0.1f + _clipSlider.getValue() * 100 / 0xFFFF);

			ESP_LOGI("ND", "Near: %d", _clipSlider.getValue() / 0xFFFF);
		};

		_slidersLayout += &_clipSlider;

		_slidersLayout.setSpacing(5);
		_slidersLayout.setVerticalAlignment(Alignment::end);
		_slidersLayout.setMargin(Margin(10, 0, 10, 10));
		*this += &_slidersLayout;
	}
}