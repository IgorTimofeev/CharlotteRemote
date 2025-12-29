#include "personalizationSettingsPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	PersonalizationSettingsPage::PersonalizationSettingsPage() {
		auto& rc = RC::getInstance();
		
		// Page title
		title.setText(L"Personalization");
		
		// LPF
		_LPFSwitcher.getSwitch().setActive(rc.getSettings().interface.LPF);
		
		_LPFSwitcher.getSwitch().isActiveChanged += [this, &rc] {
			rc.getSettings().interface.LPF = _LPFSwitcher.getSwitch().isActive();
			rc.getSettings().interface.scheduleWrite();
		};
		
		rows += &_LPFSwitcher;
		
		// Debug overlay
		_debugOverlaySwitcher.getSwitch().setActive(rc.getSettings().interface.debugOverlay);
		
		_debugOverlaySwitcher.getSwitch().isActiveChanged += [this, &rc] {
			rc.getSettings().interface.debugOverlay = _debugOverlaySwitcher.getSwitch().isActive();
			rc.getSettings().interface.scheduleWrite();
			
			rc.updateDebugOverlayVisibility();
		};
		
		rows += &_debugOverlaySwitcher;
	}
}