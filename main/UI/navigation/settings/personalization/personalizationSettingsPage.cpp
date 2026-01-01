#include "personalizationSettingsPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	PersonalizationSettingsPage::PersonalizationSettingsPage() {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();
		
		title.setText(L"General");
		
		// Audio feedback
		_audioFeedback.getSwitch().setActive(settings.personalization.audioFeedback);
		
		_audioFeedback.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.audioFeedback = _audioFeedback.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};
		
		rows += &_audioFeedback;
		
		// LPF
		_MFDLPF.getSwitch().setActive(settings.personalization.LPF);
		
		_MFDLPF.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.LPF = _MFDLPF.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};
		
		rows += &_MFDLPF;
		
		// Debug overlay
		_debugOverlay.getSwitch().setActive(settings.personalization.debugOverlay);
		
		_debugOverlay.getSwitch().isActiveChanged += [this, &rc, &settings] {
			settings.personalization.debugOverlay = _debugOverlay.getSwitch().isActive();
			settings.personalization.scheduleWrite();
			
			rc.updateDebugOverlayVisibility();
		};
		
		rows += &_debugOverlay;
		
		// Separator
		rows += &_generalSeparator;
		
		// -------------------------------- MFD --------------------------------
		
		Theme::applyPageTitle(&_MFDTitle);
		_MFDTitle.setText(L"MFD");
		rows += &_MFDTitle;
				
		// FOV
		Theme::apply(&_MFDFOVSlider);
		_MFDFOVSlider.setValue(0xFFFF * (settings.personalization.MFD.PFD.FOV - _MFDFOVMin) / (_MFDFOVMax - _MFDFOVMin));
		
		_MFDFOVSlider.valueChanged += [this, &settings] {
			settings.personalization.MFD.PFD.FOV = _MFDFOVMin + _MFDFOVSlider.getValue() * (_MFDFOVMax - _MFDFOVMin) / 0xFFFF;
			settings.personalization.scheduleWrite();
			
			updateMFDFOVTitle();
		};
		
		updateMFDFOVTitle();
		rows += &_MFDFOVTitle;
		
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
		
		// Sphere
		_MFDSphereSwitcher.getSwitch().setActive(settings.personalization.MFD.ND.earth);
		
		_MFDSphereSwitcher.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.MFD.ND.earth = _MFDSphereSwitcher.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};
		
		rows += &_MFDSphereSwitcher;
	}
	
	void PersonalizationSettingsPage::updateMFDHeightTitle() {
		_MFDSplitTitle.title.setText(std::format(L"Split view ratio ({}%)", RC::getInstance().getSettings().personalization.MFD.splitPercent));
	}
	
	void PersonalizationSettingsPage::updateMFDFOVTitle() {
		_MFDFOVTitle.title.setText(std::format(L"PFD field of view ({} deg)", RC::getInstance().getSettings().personalization.MFD.PFD.FOV));
	}
}