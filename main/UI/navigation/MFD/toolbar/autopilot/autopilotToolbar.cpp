#include "autopilotToolbar.h"
#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	AutopilotToolbar::AutopilotToolbar() {
		setHeight(36);
		
		auto& rc = RC::getInstance();

		// FD
		flightDirectorButton.setActive(rc.getSettings().personalization.MFD.PFD.flightDirector);
		
		flightDirectorButton.isActiveChanged += [this, &rc] {
			rc.getSettings().personalization.MFD.PFD.flightDirector = flightDirectorButton.isActive();
			rc.getSettings().personalization.scheduleWrite();
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &flightDirectorButton;

		// Speed
		speed.seven.setValue(rc.getSettings().autopilot.speedKt);
		speed.setActive(rc.getSettings().autopilot.autoThrottle);

		speed.rotated += [this, &rc] {
			rc.getAudioPlayer().playFeedback();
			
			rc.getSettings().autopilot.speedKt = speed.seven.getValue();
			rc.getSettings().autopilot.scheduleWrite();
		};

		speed.isActiveChanged += [this, &rc] {
			rc.getSettings().autopilot.autoThrottle = speed.isActive();
			rc.getSettings().autopilot.scheduleWrite();
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &speed;

		// Heading
		heading.seven.setValue(rc.getSettings().autopilot.headingDeg);
		heading.setActive(rc.getSettings().autopilot.headingHold);

		heading.rotated += [this, &rc] {
			rc.getSettings().autopilot.headingDeg = heading.seven.getValue();
			rc.getSettings().autopilot.scheduleWrite();
			
			rc.getAudioPlayer().playFeedback();
		};

		heading.isActiveChanged += [this, &rc] {
			rc.getSettings().autopilot.headingHold = heading.isActive();
			rc.getSettings().autopilot.scheduleWrite();
			
			rc.getAudioPlayer().playFeedback();
		};
		
		row += &heading;

		// Altitude
		altitude.seven.setValue(rc.getSettings().autopilot.altitudeFt);
		altitude.setActive(rc.getSettings().autopilot.levelChange);

		altitude.rotated += [this, &rc] {
			rc.getSettings().autopilot.altitudeFt = altitude.seven.getValue();
			rc.getSettings().autopilot.scheduleWrite();
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};

		altitude.isActiveChanged += [this, &rc] {
			rc.getSettings().autopilot.levelChange = altitude.isActive();
			rc.getSettings().autopilot.scheduleWrite();
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};
		
		row += &altitude;
		
		// A/P
		engageButton.setActive(rc.getRemoteData().raw.autopilotEngaged);
		
		engageButton.isActiveChanged += [this, &rc] {
			rc.getAudioPlayer().play(
				engageButton.isActive()
				? static_cast<const Sound&>(resources::sounds::autopilotEngaged)
				: resources::sounds::autopilotDisengaged
			);
			
			rc.getRemoteData().raw.autopilotEngaged = engageButton.isActive();
		};
		
		row += &engageButton;
	}
}