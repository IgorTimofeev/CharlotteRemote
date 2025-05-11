#include "MFDSettingsPage.h"

#include <format>
#include "../../../theme.h"
#include "../../../../constants.h"
#include "../../../../rc.h"

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
			settings.interface.enqueueWrite();

			updateMFDHeightTitle();
		};

		updateMFDHeightTitle();
		rows += &_MFDSplitTitle;

		// -------------------------------- PFD --------------------------------

		Theme::applyTitle(&_PFDTitle);
		_PFDTitle.setText(L"PFD options");
		rows += &_PFDTitle;

		// FOV
		Theme::apply(&_PFDFOVSlider);
		_PFDFOVSlider.setValue(0xFFFF * (settings.interface.MFD.PFD.FOV - _PFDFOVMin) / (_PFDFOVMax - _PFDFOVMin));

		_PFDFOVSlider.valueChanged += [this, &settings] {
			settings.interface.MFD.PFD.FOV = _PFDFOVMin + _PFDFOVSlider.getValue() * (_PFDFOVMax - _PFDFOVMin) / 0xFFFF;
			settings.interface.enqueueWrite();

			updatePFDFOVTitle();
		};

		updatePFDFOVTitle();
		rows += &_PFDFOVTitle;

		// -------------------------------- ND --------------------------------

		Theme::applyTitle(&_NDTitle);
		_NDTitle.setText(L"ND options");
		rows += &_NDTitle;

		// Sphere
		_MFDSphereSwitcher.getSwitch().setChecked(settings.interface.MFD.ND.earth);

		_MFDSphereSwitcher.getSwitch().isCheckedChanged += [this, &settings] {
			settings.interface.MFD.ND.earth = _MFDSphereSwitcher.getSwitch().isChecked();
			settings.interface.enqueueWrite();
		};

		rows += &_MFDSphereSwitcher;
	}

	void MFDSettingsPage::updateMFDHeightTitle() {
		_MFDSplitTitle.getTitle().setText(std::format(L"Split view height ({}%)", RC::getInstance().getSettings().interface.MFD.splitPercent));
	}

	void MFDSettingsPage::updatePFDFOVTitle() {
		_PFDFOVTitle.getTitle().setText(std::format(L"Field of view ({} deg)", RC::getInstance().getSettings().interface.MFD.PFD.FOV));
	}
}
