#include "lightsToolbar.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	LightsToolbar::LightsToolbar() {
		setHeight(36);
		
		row.setSpacing(1);
		
		auto& rc = RC::getInstance();
		auto& rd = rc.getRemoteData();
		
		// Navigation
		navigation.setActive(rd.raw.navigationLights);
		
		navigation.isActiveChanged += [this, &rd] {
			rd.raw.navigationLights = navigation.isActive();
		};
		
		row += &navigation;
		
		// Strobe
		strobe.setActive(rd.raw.strobeLights);
		
		strobe.isActiveChanged += [this, &rd] {
			rd.raw.strobeLights = strobe.isActive();
		};
		
		row += &strobe;
		
		// Landing
		landing.setActive(rd.raw.landingLights);
		
		landing.isActiveChanged += [this, &rd] {
			rd.raw.landingLights = landing.isActive();
		};
		
		row += &landing;
		
		// Landing
		cabin.setActive(rd.raw.cabinLights);
		
		cabin.isActiveChanged += [this, &rd] {
			rd.raw.cabinLights = cabin.isActive();
		};
		
		row += &cabin;
	}
}