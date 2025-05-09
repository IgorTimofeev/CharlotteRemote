#include "NDControls.h"
#include "../../../../rc.h"

namespace pizda {
	NDControls::NDControls() {
		*this += &_ND;

		// Right rows
		_rightRows.setSpacing(10);
		_rightRows.setAlignment(Alignment::end, Alignment::start);
		_rightRows.setMargin(Margin(0, 5, 5, 0));
		*this += &_rightRows;

		updateViewModeButtonText();

		addGovnoButton(&_viewModeButton, [this] {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFD.ND.arc = !settings.interface.MFD.ND.arc;
			settings.enqueueWrite();

			updateViewModeButtonText();
		});

		_latLongButton.setText(L"RST");

		addGovnoButton(&_latLongButton, [this] {
			_ND.resetCameraOffsetLatLon();
		});
	}

	void NDControls::addGovnoButton(Button* button, const std::function<void()>& onClick) {
		Theme::apply(button);

		button->setSize(Size(20));

		button->setDefaultBackgroundColor(&Theme::bg2);
		button->setDefaultTextColor(&Theme::fg5);

		button->setPressedBackgroundColor(&Theme::fg1);
		button->setPressedTextColor(&Theme::bg1);

		button->setFont(&Theme::fontSmall);

		button->click += onClick;

		_rightRows += button;
	}

	void NDControls::updateViewModeButtonText() {
		_viewModeButton.setText(RC::getInstance().getSettings().interface.MFD.ND.arc ? L"arc" : L"map");
	}
}