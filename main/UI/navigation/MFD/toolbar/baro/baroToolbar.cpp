#include "baroToolbar.h"

#include "rc.h"

namespace pizda {
	BaroToolbar::BaroToolbar() {
		auto& settings = RC::getInstance().getSettings();

		// Pressure
		pressure.seven.setDecimalSeparatorIndex(0);
		pressure.seven.setValue(settings.controls.referencePressurePa / 10);
		pressure.setMode(settings.controls.referencePressureSTD ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);

		pressure.rotated += [this, &settings] {
			settings.controls.referencePressurePa = pressure.seven.getValue() * 10;
			settings.controls.scheduleWrite();
			
			RC::getInstance().getPacketHandler().enqueue(RemotePacketType::baro);
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};

		pressure.pressed += [this, &settings] {
			pressure.setMode(pressure.getMode() == AutopilotValueMode::none ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);
			
			settings.controls.referencePressureSTD = pressure.getMode() != AutopilotValueMode::none;
			settings.controls.scheduleWrite();
			
			RC::getInstance().getPacketHandler().enqueue(RemotePacketType::baro);
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};
		
		row += &pressure;

		// Minimums
		minimums.seven.setValue(settings.controls.minimumAltitudeFt);
		minimums.setMode(settings.controls.minimumAltitudeEnabled ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);

		minimums.rotated += [this, &settings] {
			settings.controls.minimumAltitudeFt = minimums.seven.getValue();
			settings.controls.scheduleWrite();
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};

		minimums.pressed += [this, &settings] {
			minimums.setMode(minimums.getMode() == AutopilotValueMode::none ? AutopilotValueMode::acknowledged : AutopilotValueMode::none);
			
			settings.controls.minimumAltitudeFt = minimums.getMode() != AutopilotValueMode::none;
			settings.controls.scheduleWrite();
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};
		
		row += &minimums;
	}
}