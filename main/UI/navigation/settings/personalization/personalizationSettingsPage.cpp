#include "personalizationSettingsPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	PersonalizationSettingsPage::PersonalizationSettingsPage() {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// ----------------------------- General -----------------------------

		title.setText(L"General");

		// Audio feedback
		_generalAudioFeedback.getSwitch().setActive(settings.personalization.audioFeedback);
		
		_generalAudioFeedback.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.audioFeedback = _generalAudioFeedback.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};
		
		rows += &_generalAudioFeedback;
		
		// LPF
		_generalDataInterpolation.getSwitch().setActive(settings.personalization.LPF);
		
		_generalDataInterpolation.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.LPF = _generalDataInterpolation.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};
		
		rows += &_generalDataInterpolation;
		
		// Debug overlay
		_generalDebugOverlay.getSwitch().setActive(settings.personalization.debugOverlay);
		
		_generalDebugOverlay.getSwitch().isActiveChanged += [this, &rc, &settings] {
			settings.personalization.debugOverlay = _generalDebugOverlay.getSwitch().isActive();
			settings.personalization.scheduleWrite();
			
			rc.updateDebugOverlayVisibility();
		};
		
		rows += &_generalDebugOverlay;
		
		// Separator
		rows += &_generalSeparator;
		
		// -------------------------------- PFD --------------------------------

		Theme::applyPageTitle(&_PFDTitle);
		_PFDTitle.setText(L"PFD");
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
		
		// Height
		Theme::apply(&_PFDSplitSlider);
		_PFDSplitSlider.setValue(0xFFFF * (settings.personalization.MFD.splitPercent - _PFDSplitMin) / (_PFDSplitMax - _PFDSplitMin));
		
		_PFDSplitSlider.valueChanged += [this, &settings] {
			settings.personalization.MFD.splitPercent = _PFDSplitMin + _PFDSplitSlider.getValue() * (_PFDSplitMax - _PFDSplitMin) / 0xFFFF;
			settings.personalization.scheduleWrite();
			
			updatePFDSplitTitle();
		};
		
		updatePFDSplitTitle();
		rows += &_PFDSplitTitle;

		// Waypoint labels
		_PFDWaypointLabels.getSwitch().setActive(settings.personalization.MFD.PFD.waypointLabels);

		_PFDWaypointLabels.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.MFD.PFD.waypointLabels = _PFDWaypointLabels.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};

		rows += &_PFDWaypointLabels;

		// Separator
		rows += &_PFDSeparator;

		// -------------------------------- ND --------------------------------

		Theme::applyPageTitle(&_NDTitle);
		_NDTitle.setText(L"ND");
		rows += &_NDTitle;

		// Earth grid
		_NDEarthGrid.getSwitch().setActive(settings.personalization.MFD.ND.earth);
		
		_NDEarthGrid.getSwitch().isActiveChanged += [this, &settings] {
			settings.personalization.MFD.ND.earth = _NDEarthGrid.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		};
		
		rows += &_NDEarthGrid;
	}
	
	void PersonalizationSettingsPage::updatePFDSplitTitle() {
		_PFDSplitTitle.title.setText(std::format(L"Split view height ({}%)", RC::getInstance().getSettings().personalization.MFD.splitPercent));
	}
	
	void PersonalizationSettingsPage::updatePFDFOVTitle() {
		_PFDFOVTitle.title.setText(std::format(L"Field of view ({} deg)", RC::getInstance().getSettings().personalization.MFD.PFD.FOV));
	}
}