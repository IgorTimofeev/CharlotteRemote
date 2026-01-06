#include "lightsToolbar.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	LightsToolbar::LightsToolbar() {
		row.setSpacing(1);
		
		auto& rc = RC::getInstance();
		
		// Navigation
		navigation.pressed += [this, &rc] {
			rc.getRemoteData().lights.navigation = navigation.isActive();
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &navigation;
		
		// Strobe
		strobe.pressed += [this, &rc] {
			rc.getRemoteData().lights.strobe = strobe.isActive();
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &strobe;
		
		// Landing
		landing.pressed += [this, &rc] {
			rc.getRemoteData().lights.landing = landing.isActive();
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &landing;
		
		// Landing
		cabin.pressed += [this, &rc] {
			rc.getRemoteData().lights.cabin = cabin.isActive();
			
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