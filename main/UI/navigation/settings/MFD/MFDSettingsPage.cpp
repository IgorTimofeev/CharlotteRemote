#include "MFDSettingsPage.h"

#include <format>
#include <UI/theme.h>
#include <constants.h>
#include <rc.h>

namespace pizda {
	MFDSettingsPage::MFDSettingsPage() {
		auto& settings = RC::getInstance().getSettings();

		// -------------------------------- MFD --------------------------------

		title.setText(L"MFD options");

		// Height
		Theme::apply(&_MFDSplitSlider);
		_MFDSplitSlider.setValue(0xFFFF * (settings.interface.MFD.splitPercent - _MFDSplitMin) / (_MFDSplitMax - _MFDSplitMin));

		_MFDSplitSlider.valueChanged += [this, &settings] {
			settings.interface.MFD.splitPercent = _MFDSplitMin + _MFDSplitSlider.getValue() * (_MFDSplitMax - _MFDSplitMin) / 0xFFFF;
			settings.interface.scheduleWrite();

			updateMFDHeightTitle();
		};

		updateMFDHeightTitle();
		rows += &_MFDSplitTitle;

		// -------------------------------- PFD --------------------------------

		Theme::applyPageTitle(&_PFDTitle);
		_PFDTitle.setText(L"PFD options");
		rows += &_PFDTitle;

		// FOV
		Theme::apply(&_PFDFOVSlider);
		_PFDFOVSlider.setValue(0xFFFF * (settings.interface.MFD.PFD.FOV - _PFDFOVMin) / (_PFDFOVMax - _PFDFOVMin));

		_PFDFOVSlider.valueChanged += [this, &settings] {
			settings.interface.MFD.PFD.FOV = _PFDFOVMin + _PFDFOVSlider.getValue() * (_PFDFOVMax - _PFDFOVMin) / 0xFFFF;
			settings.interface.scheduleWrite();

			updatePFDFOVTitle();
		};

		updatePFDFOVTitle();
		rows += &_PFDFOVTitle;

		// -------------------------------- ND --------------------------------

		Theme::applyPageTitle(&_NDTitle);
		_NDTitle.setText(L"ND options");
		rows += &_NDTitle;

		// Sphere
		_MFDSphereSwitcher.getSwitch().setActive(settings.interface.MFD.ND.earth);

		_MFDSphereSwitcher.getSwitch().isActiveChanged += [this, &settings] {
			settings.interface.MFD.ND.earth = _MFDSphereSwitcher.getSwitch().isActive();
			settings.interface.scheduleWrite();
		};

		rows += &_MFDSphereSwitcher;
	}

	void MFDSettingsPage::updateMFDHeightTitle() {
		_MFDSplitTitle.title.setText(std::format(L"Split view height ({}%)", RC::getInstance().getSettings().interface.MFD.splitPercent));
	}

	void MFDSettingsPage::updatePFDFOVTitle() {
		_PFDFOVTitle.title.setText(std::format(L"Field of view ({} deg)", RC::getInstance().getSettings().interface.MFD.PFD.FOV));
	}
}
