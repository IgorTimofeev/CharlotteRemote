#include "MFDSettingsPage.h"

#include <format>
#include "../../../theme.h"
#include "../../../../constants.h"
#include "../../../../rc.h"

namespace pizda {
	MFDSettingsPage::MFDSettingsPage() {
		auto& settings = RC::getInstance().getSettings();

		// Title
		title.setText(L"PFD options");

		Theme::apply(&_PFDFOVSlider);
		_PFDFOVSlider.setValue(0xFFFF * (settings.interface.MFD.PFD.FOV - _PFDFOVMin) / (_PFDFOVMax - _PFDFOVMin));

		_PFDFOVSlider.valueChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFD.PFD.FOV = _PFDFOVMin + _PFDFOVSlider.getValue() * (_PFDFOVMax - _PFDFOVMin) / 0xFFFF;
			settings.enqueueWrite();

			updatePFDFOVTitle();
		};

		updatePFDFOVTitle();
		rows += &_PFDFOVTitle;

		// MFD
		Theme::applyTitle(&_MFDTitle);
		_MFDTitle.setText(L"ND options");
		rows += &_MFDTitle;

		_MFDSphereSwitcher.getSwitch().setChecked(true);
		rows += &_MFDSphereSwitcher;
	}

	void MFDSettingsPage::updatePFDFOVTitle() {
		_PFDFOVTitle.getTitle().setText(std::format(L"Field of view ({} deg)", RC::getInstance().getSettings().interface.MFD.PFD.FOV));
	}
}