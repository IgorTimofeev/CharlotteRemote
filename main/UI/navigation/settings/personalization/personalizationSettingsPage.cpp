#include "personalizationSettingsPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	PersonalizationSettingsPage::PersonalizationSettingsPage() {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// ----------------------------- General -----------------------------

		title.setText(L"General");

		// Debug overlay
		_generalDebugOverlay.getSwitch().setActive(settings.personalization.debugOverlay);

		_generalDebugOverlay.getSwitch().setOnIsActiveChanged([this, &rc, &settings] {
			settings.personalization.debugOverlay = _generalDebugOverlay.getSwitch().isActive();
			settings.personalization.scheduleWrite();

			rc.updateDebugOverlayVisibility();
		});

		rows += _generalDebugOverlay;

		// Audio feedback
		_generalAudioFeedback.getSwitch().setActive(settings.personalization.audioFeedback);
		
		_generalAudioFeedback.getSwitch().setOnIsActiveChanged([this, &settings] {
			settings.personalization.audioFeedback = _generalAudioFeedback.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		});
		
		rows += _generalAudioFeedback;
		
		// LPF
		_generalDataInterpolation.getSwitch().setActive(settings.personalization.LPF);
		
		_generalDataInterpolation.getSwitch().setOnIsActiveChanged([this, &settings] {
			settings.personalization.LPF = _generalDataInterpolation.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		});
		
		rows += _generalDataInterpolation;
		
		// Separator
		rows += _generalSeparator;
		
		// -------------------------------- PFD --------------------------------

		Theme::applyPageTitle(&_PFDTitle);
		_PFDTitle.setText(L"PFD");
		rows += _PFDTitle;
				
		// FOV
		Theme::apply(&_PFDFOVSlider);
		_PFDFOVSlider.setMinimumValue(20);
		_PFDFOVSlider.setMaximumValue(120);
		_PFDFOVSlider.setValue(settings.personalization.MFD.PFD.FOV);

		_PFDFOVSlider.setTickCount(10);
		_PFDFOVSlider.setBigTickStep(5);
		_PFDFOVSlider.setTickLabelBuilder(Slider::int32TickLabelBuilder);

		_PFDFOVSlider.setOnValueChanged([this, &settings] {
			settings.personalization.MFD.PFD.FOV = _PFDFOVSlider.getValue();
			settings.personalization.scheduleWrite();
		});

		rows += _PFDFOVTitle;

		// Waypoint labels
		_PFDWaypointLabels.getSwitch().setActive(settings.personalization.MFD.PFD.waypointLabels);

		_PFDWaypointLabels.getSwitch().setOnIsActiveChanged([this, &settings] {
			settings.personalization.MFD.PFD.waypointLabels = _PFDWaypointLabels.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		});

		rows += _PFDWaypointLabels;

		// Separator
		rows += _PFDSeparator;

		// -------------------------------- ND --------------------------------

		Theme::applyPageTitle(&_NDTitle);
		_NDTitle.setText(L"ND");
		rows += _NDTitle;

		// Earth grid
		_NDEarthGrid.getSwitch().setActive(settings.personalization.MFD.ND.earth);
		
		_NDEarthGrid.getSwitch().setOnIsActiveChanged([this, &settings] {
			settings.personalization.MFD.ND.earth = _NDEarthGrid.getSwitch().isActive();
			settings.personalization.scheduleWrite();
		});
		
		rows += _NDEarthGrid;
	}
}