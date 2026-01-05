#include "autopilotToolbar.h"
#include "rc.h"
#include "resources/sounds.h"

#include "UI/navigation/MFD/toolbar/autopilot/modeDialog.h"

namespace pizda {
	AutopilotToolbar::AutopilotToolbar() {
		auto& rc = RC::getInstance();
	
		// FD
		flightDirector.setMode(rc.getSettings().personalization.MFD.PFD.flightDirector ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);
		
		flightDirector.pressed += [this, &rc] {
			flightDirector.setMode(flightDirector.getMode() == AutopilotValueMode::acknowledged ? AutopilotValueMode::none : AutopilotValueMode::acknowledged);
			
			rc.getSettings().personalization.MFD.PFD.flightDirector = flightDirector.getMode() == AutopilotValueMode::acknowledged;
			rc.getSettings().personalization.scheduleWrite();
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &flightDirector;

		// Speed
		speed.seven.setValue(rc.getSettings().autopilot.speedKt);

		speed.rotated += [this, &rc] {
			rc.getSettings().autopilot.speedKt = speed.seven.getValue();
			rc.getSettings().autopilot.scheduleWrite();
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().playFeedback();
		};

		speed.pressed += [&rc] {
			(new ModeDialog())->show();
			
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
			(new ModeDialog())->show();
			
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
			(new ModeDialog())->show();
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &altitude;
		
		// Autopilot
		autopilot.pressed += [&rc] {
			rc.getRemoteData().autopilot.autopilot = !rc.getRemoteData().autopilot.autopilot;
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			
			rc.getAudioPlayer().play(
				rc.getRemoteData().autopilot.autopilot
				? static_cast<const Sound&>(resources::sounds::autopilotEngaged)
				: resources::sounds::autopilotDisengaged
			);
		};
		
		row += &autopilot;
	}
	
	void AutopilotToolbar::onTick() {
		Layout::onTick();
		
		auto& rc = RC::getInstance();
		
		if (rc.getRemoteData().autopilot.autothrottle) {
			if (rc.getRemoteData().autopilot.autothrottle == rc.getAircraftData().raw.autopilot.autothrottle) {
				speed.setMode(AutopilotValueMode::acknowledged);
			}
			else {
				speed.setMode(AutopilotValueMode::selected);
			}
		}
		else {
			speed.setMode(AutopilotValueMode::none);
		}
		
		if (rc.getRemoteData().autopilot.lateralMode == AutopilotLateralMode::man) {
			heading.setMode(AutopilotValueMode::none);
		}
		else {
			if (rc.getRemoteData().autopilot.lateralMode == rc.getAircraftData().raw.autopilot.lateralMode) {
				heading.setMode(AutopilotValueMode::acknowledged);
			}
			else {
				heading.setMode(AutopilotValueMode::selected);
			}
		}
		
		if (rc.getRemoteData().autopilot.verticalMode == AutopilotVerticalMode::man) {
			altitude.setMode(AutopilotValueMode::none);
		}
		else {
			if (rc.getRemoteData().autopilot.verticalMode == rc.getAircraftData().raw.autopilot.verticalMode) {
				altitude.setMode(AutopilotValueMode::acknowledged);
			}
			else {
				altitude.setMode(AutopilotValueMode::selected);
			}
		}
		
		if (rc.getRemoteData().autopilot.autopilot) {
			if (rc.getRemoteData().autopilot.autopilot == rc.getAircraftData().raw.autopilot.autopilot) {
				autopilot.setMode(AutopilotValueMode::acknowledged);
			}
			else {
				autopilot.setMode(AutopilotValueMode::selected);
			}
		}
		else {
			autopilot.setMode(AutopilotValueMode::none);
		}
	}
}