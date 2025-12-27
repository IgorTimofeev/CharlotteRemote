#include "lightsToolbar.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	LightsToolbar::LightsToolbar() {
		setHeight(36);
		
		auto& settings = RC::getInstance().getSettings();
		
		// Navigation
		navigation.setToggle(true);
		navigation.setActive(settings.controls.navigationLights);
		
		navigation.isActiveChanged += [this, &settings] {
			settings.controls.navigationLights = navigation.isActive();
			settings.controls.scheduleWrite();
		};
		
		row += &navigation;
		
		// Strobe
		strobe.setToggle(true);
		strobe.setActive(settings.controls.strobeLights);
		
		strobe.isActiveChanged += [this, &settings] {
			settings.controls.strobeLights = strobe.isActive();
			settings.controls.scheduleWrite();
		};
		
		row += &strobe;
		
		// Landing
		landing.setToggle(true);
		landing.setActive(settings.controls.landingLights);
		
		landing.isActiveChanged += [this, &settings] {
			settings.controls.landingLights = landing.isActive();
			settings.controls.scheduleWrite();
		};
		
		row += &landing;
	}
}