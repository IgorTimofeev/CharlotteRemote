#include "autopilotToolbar.h"
#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	AutopilotToolbar::AutopilotToolbar() {
		setHeight(36);
		
		auto& rc = RC::getInstance();
	
		// FD
		flightDirectorButton.setActive(rc.getSettings().personalization.MFD.PFD.flightDirector);
		
		flightDirectorButton.pressed += [this, &rc] {
			flightDirectorButton.setActive(!flightDirectorButton.isActive());
			
			rc.getSettings().personalization.MFD.PFD.flightDirector = flightDirectorButton.isActive();
			rc.getSettings().personalization.scheduleWrite();
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &flightDirectorButton;

		// Speed
		speed.seven.setValue(rc.getSettings().autopilot.speedKt);

		speed.rotated += [this, &rc] {
			rc.getSettings().autopilot.speedKt = speed.seven.getValue();
			rc.getSettings().autopilot.scheduleWrite();
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().playFeedback();
		};

		speed.pressed += [&rc] {
			rc.getRemoteData().autopilot.autothrottle = !rc.getRemoteData().autopilot.autothrottle;
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &speed;

		// Heading
		heading.seven.setValue(rc.getSettings().autopilot.headingDeg);

		heading.rotated += [this, &rc] {
			rc.getSettings().autopilot.headingDeg = heading.seven.getValue();
			rc.getSettings().autopilot.scheduleWrite();
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().playFeedback();
		};

		heading.pressed += [&rc] {
			switch (rc.getRemoteData().autopilot.lateralMode) {
				case AutopilotLateralMode::roll: {
					rc.getRemoteData().autopilot.lateralMode = AutopilotLateralMode::heading;
					break;
				}
				default: {
					rc.getRemoteData().autopilot.lateralMode = AutopilotLateralMode::roll;
					break;
				}
			}
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &heading;

		// Altitude
		altitude.seven.setValue(rc.getSettings().autopilot.altitudeFt);

		altitude.rotated += [this, &rc] {
			rc.getSettings().autopilot.altitudeFt = altitude.seven.getValue();
			rc.getSettings().autopilot.scheduleWrite();
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};
		
		altitude.pressed += [&rc] {
			switch (rc.getRemoteData().autopilot.verticalMode) {
				case AutopilotVerticalMode::pitch: {
					rc.getRemoteData().autopilot.verticalMode = AutopilotVerticalMode::levelChange;
					break;
				}
				default: {
					rc.getRemoteData().autopilot.verticalMode = AutopilotVerticalMode::pitch;
					break;
				}
			}
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &altitude;
		
		// Autopilot
		engageButton.pressed += [&rc] {
			rc.getRemoteData().autopilot.autopilot = !rc.getRemoteData().autopilot.autopilot;
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().play(
				rc.getRemoteData().autopilot.autopilot
				? static_cast<const Sound&>(resources::sounds::autopilotEngaged)
				: resources::sounds::autopilotDisengaged
			);
		};
		
		row += &engageButton;
	}
	
	void AutopilotToolbar::onTick() {
		Layout::onTick();
		
		auto& rc = RC::getInstance();
		
		speed.setActive(rc.getAircraftData().raw.autopilot.autothrottle);
		heading.setActive(rc.getAircraftData().raw.autopilot.lateralMode == AutopilotLateralMode::heading);
		altitude.setActive(rc.getAircraftData().raw.autopilot.verticalMode != AutopilotVerticalMode::pitch);
		engageButton.setActive(rc.getAircraftData().raw.autopilot.autopilot);
	}
}