#include "lightsToolbar.h"
#include "rc.h"

namespace pizda {
	using namespace YOBA;

	LightsToolbar::LightsToolbar() {
		row.setSpacing(1);
		
		auto& rc = RC::getInstance();
		
		// Navigation
		navigation.pressed += [&rc] {
			rc.getRemoteData().lights.navigation = !rc.getRemoteData().lights.navigation;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &navigation;
		
		// Strobe
		strobe.pressed += [&rc] {
			rc.getRemoteData().lights.strobe = !rc.getRemoteData().lights.strobe;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &strobe;
		
		// Landing
		landing.pressed += [&rc] {
			rc.getRemoteData().lights.landing = !rc.getRemoteData().lights.landing;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &landing;
		
		// Landing
		cabin.pressed += [&rc] {
			rc.getRemoteData().lights.cabin = !rc.getRemoteData().lights.cabin;
			
			rc.getPacketHandler().enqueue(RemotePacketType::lights);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &cabin;
	}
	
	void LightsToolbar::onTick() {
		Layout::onTick();
		
		auto& rc = RC::getInstance();
		
		navigation.setMode(rc.getAircraftData().raw.lights.navigation ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);
		strobe.setMode(rc.getAircraftData().raw.lights.strobe ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);
		landing.setMode(rc.getAircraftData().raw.lights.landing ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);
		cabin.setMode(rc.getAircraftData().raw.lights.cabin ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);
	}
}