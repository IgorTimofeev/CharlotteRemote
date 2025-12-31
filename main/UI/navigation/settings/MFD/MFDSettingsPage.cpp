#include "MFDSettingsPage.h"

#include <format>
#include "UI/theme.h"
#include "config.h"
#include "rc.h"

namespace pizda {
	MFDSettingsPage::MFDSettingsPage() {
		auto& settings = RC::getInstance().getSettings();

		// -------------------------------- MFD --------------------------------

		title.setText(L"MFD options");

		// Height
		Theme::apply(&_MFDSplitSlider);
		_MFDSplitSlider.setValue(0xFFFF * (settings.personalization.MFD.splitPercent - _MFDSplitMin) / (_MFDSplitMax - _MFDSplitMin));

		_MFDSplitSlider.valueChanged += [this, &settings] {
			settings.personalization.MFD.splitPercent = _MFDSplitMin + _MFDSplitSlider.getValue() * (_MFDSplitMax - _MFDSplitMin) / 0xFFFF;
			settings.personalization.scheduleWrite();

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
		_PFDFOVSlider.setValue(0xFFFF * (settings.personalization.MFD.PFD.FOV - _PFDFOVMin) / (_PFDFOVMax - _PFDFOVMin));

		_PFDFOVSlider.valueChanged += [this, &settings] {
			settings.personalization.MFD.PFD.FOV = _PFDFOVMin + _PFDFOVSlider.getValue() * (_PFDFOVMax - _PFDFOVMin) / 0xFFFF;
			settings.personalization.scheduleWrite();

			updatePFDFOVTitle();
		};

		updatePFDFOVTitle();
		rows += &_PFDFOVTitle;

		// -------------------------------- ND --------------------------------

		Theme::applyPageTitle(&_NDTitle);
		_NDTitle.setText(L"ND options");
		rows += &_NDTitle;

		// Sphere
		_MFDSphereSwitcher.getSwitch().setActive(settings.personalization.MFD.ND.earth);

		_MFDSphereSwitcher.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.MFD.ND.earth = _MFDSphereSwitcher.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};

		rows += &_MFDSphereSwitcher;
	}

	void MFDSettingsPage::updateMFDHeightTitle() {
		_MFDSplitTitle.title.setText(std::format(L"Split view height ({}%)", RC::getInstance().getSettings().personalization.MFD.splitPercent));
	}

	void MFDSettingsPage::updatePFDFOVTitle() {
		_PFDFOVTitle.title.setText(std::format(L"Field of view ({} deg)", RC::getInstance().getSettings().personalization.MFD.PFD.FOV));
	}
}
