#include "lightsToolbar.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	LightsToolbar::LightsToolbar() {
		row.setSpacing(1);
		
		// Navigation
		navigation.pressed += [this] {
			auto& rc = RC::getInstance();
			
			copyAircraftToRemote();
			rc.getRemoteData().lights.navigation = !rc.getAircraftData().raw.lights.navigation;
			
			rc.getCommunicationManager().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &navigation;
		
		// Strobe
		strobe.pressed += [this] {
			auto& rc = RC::getInstance();
			
			copyAircraftToRemote();
			rc.getRemoteData().lights.strobe = !rc.getAircraftData().raw.lights.strobe;
			
			rc.getCommunicationManager().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &strobe;
		
		// Landing
		landing.pressed += [this] {
			auto& rc = RC::getInstance();
			
			copyAircraftToRemote();
			rc.getRemoteData().lights.landing = !rc.getAircraftData().raw.lights.landing;
			
			rc.getCommunicationManager().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &landing;
		
		// Landing
		cabin.pressed += [this] {
			auto& rc = RC::getInstance();
			
			copyAircraftToRemote();
			rc.getRemoteData().lights.cabin = !rc.getAircraftData().raw.lights.cabin;
			
			rc.getCommunicationManager().enqueue(RemotePacketType::lights);
			
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
	
	void LightsToolbar::copyAircraftToRemote() {
		auto& rc = RC::getInstance();
		
		rc.getRemoteData().lights.navigation = rc.getAircraftData().raw.lights.navigation;
		rc.getRemoteData().lights.strobe = rc.getAircraftData().raw.lights.strobe;
		rc.getRemoteData().lights.landing = rc.getAircraftData().raw.lights.landing;
		rc.getRemoteData().lights.cabin = rc.getAircraftData().raw.lights.cabin;
	}
}