#include "modeDialog.h"

#include "rc.h"

namespace pizda {
	ModeDialog::ModeDialog() {
		auto& rc = RC::getInstance();
		
		// Wrap
		wrapLayout.setSpacing(10);
		wrapLayout.setMargin(Margin(15));
		wrapLayout.setHorizontalAlignment(Alignment::center);
		
		slideLayout += &wrapLayout;
		
		// -------------------------------- Throttle --------------------------------
		
		// HDG
		ATButton.click += [this, &rc]() {
			rc.getRemoteData().autopilot.autothrottle = ATButton.getMode() == AutopilotValueMode::selected;
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			rc.getAudioPlayer().playFeedback();
		};
		
		wrapLayout += &ATButton;
		
		// -------------------------------- Lateral --------------------------------
		
		// HDG
		HDGButton.click += [this, &rc]() {
			rc.getRemoteData().autopilot.lateralMode = HDGButton.getMode() == AutopilotValueMode::selected ? AutopilotLateralMode::hdg : AutopilotLateralMode::man;
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			rc.getAudioPlayer().playFeedback();
		};
		
		wrapLayout += &HDGButton;
		
		// -------------------------------- Vertical --------------------------------
		
		// FLC
		FLCButton.click += [this, &rc]() {
			ALTButton.setMode(AutopilotValueMode::none);
			
			rc.getRemoteData().autopilot.altitudeHoldFt = 0;
			rc.getRemoteData().autopilot.verticalMode = FLCButton.getMode() == AutopilotValueMode::selected ? AutopilotVerticalMode::flc : AutopilotVerticalMode::man;

			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			rc.getAudioPlayer().playFeedback();
		};
		
		wrapLayout += &FLCButton;
		
		// ALT
		ALTButton.click += [this, &rc]() {
			FLCButton.setMode(AutopilotValueMode::none);
			
			rc.getRemoteData().autopilot.altitudeHoldFt = rc.getAircraftData().computed.altitudeFt;
			rc.getRemoteData().autopilot.verticalMode = ALTButton.getMode() == AutopilotValueMode::selected ? AutopilotVerticalMode::alt : AutopilotVerticalMode::man;
			
			rc.getPacketHandler().enqueue(RemotePacketType::autopilot);
			rc.getAudioPlayer().playFeedback();
		};
		
		wrapLayout += &ALTButton;
	}
	
	void ModeDialog::onTick() {
		Layout::onTick();
		
		auto& rc = RC::getInstance();
		
		ATButton.setMode(
			rc.getRemoteData().autopilot.autothrottle
			? (
				rc.getRemoteData().autopilot.autothrottle == rc.getAircraftData().raw.autopilot.autothrottle
				? AutopilotValueMode::acknowledged
				: AutopilotValueMode::selected
			)
			: AutopilotValueMode::none
		);
		
		if (rc.getRemoteData().autopilot.lateralMode == AutopilotLateralMode::hdg) {
			if (HDGButton.getMode() == AutopilotValueMode::selected) {
				HDGButton.setMode(AutopilotValueMode::acknowledged);
			}
		}
		else {
			if (HDGButton.getMode() != AutopilotValueMode::selected) {
				HDGButton.setMode(AutopilotValueMode::none);
			}
		}
		
		if (rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::flc) {
			if (FLCButton.getMode() == AutopilotValueMode::selected) {
				FLCButton.setMode(AutopilotValueMode::acknowledged);
			}
			
			if (ALTButton.getMode() == AutopilotValueMode::selected) {
				ALTButton.setMode(AutopilotValueMode::none);
			}
		}
		else {
			if (FLCButton.getMode() != AutopilotValueMode::selected) {
				FLCButton.setMode(AutopilotValueMode::none);
			}
		}
		
		if (rc.getRemoteData().autopilot.verticalMode == AutopilotVerticalMode::alt || rc.getRemoteData().autopilot.verticalMode == AutopilotVerticalMode::alts) {
			if (FLCButton.getMode() == AutopilotValueMode::selected) {
				FLCButton.setMode(AutopilotValueMode::none);
			}
			
			if (ALTButton.getMode() == AutopilotValueMode::selected) {
				ALTButton.setMode(AutopilotValueMode::acknowledged);
			}
		}
		else {
			if (ALTButton.getMode() != AutopilotValueMode::selected) {
				ALTButton.setMode(AutopilotValueMode::none);
			}
		}
	}
}