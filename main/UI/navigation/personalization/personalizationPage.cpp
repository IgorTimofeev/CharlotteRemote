#include "personalizationPage.h"

#include <format>
#include "../../theme.h"
#include "../../../constants.h"
#include "../../../rc.h"

namespace pizda {
	PersonalizationPage::PersonalizationPage() {
		auto& settings = RC::getInstance().getSettings();

		// Title
		title.setText(L"Personalization");

		Theme::apply(&_FOVSlider);
		_FOVSlider.setValue(0xFFFF * (settings.interface.MFD.PFD.FOV - FOVMin) / (FOVMax - FOVMin));

		_FOVSlider.valueChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFD.PFD.FOV = FOVMin + _FOVSlider.getValue() * (FOVMax - FOVMin) / 0xFFFF;
			settings.enqueueWrite();

			updateFOVTitle();
		};

		updateFOVTitle();
		rows += &_FOVTitle;
	}

	void PersonalizationPage::updateFOVTitle() {
		_FOVTitle.getTitle().setText(std::format(L"PFD field of view ({} deg)", RC::getInstance().getSettings().interface.MFD.PFD.FOV));
	}
}