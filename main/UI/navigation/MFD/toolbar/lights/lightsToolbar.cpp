#include "lightsToolbar.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	LightsToolbar::LightsToolbar() {
		row.setSpacing(1);
		
		auto& rc = RC::getInstance();
		
		// Navigation
		navigation.pressed += [this, &rc] {
			rc.getRemoteData().lights.navigation = !rc.getAircraftData().raw.lights.navigation;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &navigation;
		
		// Strobe
		strobe.pressed += [this, &rc] {
			rc.getRemoteData().lights.strobe = !rc.getAircraftData().raw.lights.strobe;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &strobe;
		
		// Landing
		landing.pressed += [this, &rc] {
			rc.getRemoteData().lights.landing = !rc.getAircraftData().raw.lights.landing;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &landing;
		
		// Landing
		cabin.pressed += [this, &rc] {
			rc.getRemoteData().lights.cabin = !rc.getAircraftData().raw.lights.cabin;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &cabin;
	}
	
	void LightsToolbar::onTick() {
		Layout::onTick();
		
		auto& rc = RC::getInstance();
		
		navigation.setActive(rc.getAircraftData().raw.lights.navigation);
		strobe.setActive(rc.getAircraftData().raw.lights.strobe);
		landing.setActive(rc.getAircraftData().raw.lights.landing);
		cabin.setActive(rc.getAircraftData().raw.lights.cabin);
	}
}