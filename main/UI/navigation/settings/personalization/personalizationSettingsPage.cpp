#include "personalizationSettingsPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	PersonalizationSettingsPage::PersonalizationSettingsPage() {
		auto& rc = RC::getInstance();
		
		// Page title
		title.setText(L"Personalization");
		
		// LPF
		_LPF.getSwitch().setActive(rc.getSettings().personalization.LPF);
		
		_LPF.getSwitch().isActiveChanged += [this, &rc] {
			rc.getSettings().personalization.LPF = _LPF.getSwitch().isActive();
			rc.getSettings().personalization.scheduleWrite();
		};
		
		rows += &_LPF;
		
		// LPF
		_audioFeedback.getSwitch().setActive(rc.getSettings().personalization.audioFeedback);
		
		_audioFeedback.getSwitch().isActiveChanged += [this, &rc] {
			rc.getSettings().personalization.audioFeedback = _audioFeedback.getSwitch().isActive();
			rc.getSettings().personalization.scheduleWrite();
		};
		
		rows += &_audioFeedback;
		
		// Debug overlay
		_debugOverlay.getSwitch().setActive(rc.getSettings().personalization.debugOverlay);
		
		_debugOverlay.getSwitch().isActiveChanged += [this, &rc] {
			rc.getSettings().personalization.debugOverlay = _debugOverlay.getSwitch().isActive();
			rc.getSettings().personalization.scheduleWrite();
			
			rc.updateDebugOverlayVisibility();
		};
		
		rows += &_debugOverlay;
	}
}