#include "ndPage.h"

namespace pizda {
	NDPage::NDPage() {
		*this += &_nd;

		// FOV
		Theme::apply(&_fovSlider);

		_fovSlider.valueChanged += [this]() {
			_nd.getCamera().setFieldOfView(_fovSlider.getValue() * yoba::toRadians(180));
		};

		_slidersLayout += &_fovSlider;

		// CLip
		Theme::apply(&_clipSlider);

		_clipSlider.valueChanged += [this]() {
			_nd.getCamera().setNearPlaneDistance(1 + _clipSlider.getValue() * 1000);
		};

		_slidersLayout += &_clipSlider;

		_slidersLayout.setSpacing(5);
		_slidersLayout.setVerticalAlignment(Alignment::end);
		_slidersLayout.setMargin(Margin(10, 0, 10, 10));
		*this += &_slidersLayout;
	}
}