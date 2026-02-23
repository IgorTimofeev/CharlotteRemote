#include "autopilotToolbar.h"
#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	AutopilotToolbar::AutopilotToolbar() {
		auto& rc = RC::getInstance();
	
		// FD
		flightDirector.setActive(rc.getSettings().personalization.MFD.PFD.flightDirector);

		flightDirector.onPressed = [this, &rc] {
			rc.getSettings().personalization.MFD.PFD.flightDirector = !rc.getSettings().personalization.MFD.PFD.flightDirector;
			rc.getSettings().personalization.scheduleWrite();
			
			flightDirector.setActive(rc.getSettings().personalization.MFD.PFD.flightDirector);
			
			rc.playFeedback();
		};

		row += &flightDirector;
		
		row += &speed;
		row += &lateral;
		row += &vertical;

		// Autopilot
		autopilot.onPressed = [&rc] {
			rc.getRemoteData().autopilot.autopilot = !RC::getInstance().getAircraftData().raw.autopilot.autopilot;

			rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setAutopilotEngaged);

			rc.getAudioPlayer().play(
				rc.getRemoteData().autopilot.autopilot
				? static_cast<const Sound*>(&resources::sounds::engaged)
				: &resources::sounds::disengaged
			);
		};

		row += &autopilot;
	}
	
	void AutopilotToolbar::onTick() {
		Layout::onTick();
		
		autopilot.setActive(RC::getInstance().getAircraftData().raw.autopilot.autopilot);
	}
}