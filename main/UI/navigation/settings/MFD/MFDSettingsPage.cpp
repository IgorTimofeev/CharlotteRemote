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

		_PFDFOVSlider.valueChanged += [this, &settings] {
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

		// Slider
		Theme::apply(&_MFDHeightSlider);
		_MFDHeightSlider.setValue(0xFFFF * (settings.interface.MFD.ND.heightPercent - _MFDHeightMin) / (_MFDHeightMax - _MFDHeightMin));

		_MFDHeightSlider.valueChanged += [this, &settings] {
			settings.interface.MFD.ND.heightPercent = _MFDHeightMin + _MFDHeightSlider.getValue() * (_MFDHeightMax - _MFDHeightMin) / 0xFFFF;
			settings.enqueueWrite();

			updateMFDHeightTitle();
		};

		updateMFDHeightTitle();
		rows += &_MFDHeightTitle;

		// Sphere
		_MFDSphereSwitcher.getSwitch().setChecked(true);
		rows += &_MFDSphereSwitcher;
	}

	void MFDSettingsPage::updatePFDFOVTitle() {
		_PFDFOVTitle.getTitle().setText(std::format(L"Field of view ({} deg)", RC::getInstance().getSettings().interface.MFD.PFD.FOV));
	}

	void MFDSettingsPage::updateMFDHeightTitle() {
		_MFDHeightTitle.getTitle().setText(std::format(L"Height ({} %)", RC::getInstance().getSettings().interface.MFD.ND.heightPercent));
	}
}
