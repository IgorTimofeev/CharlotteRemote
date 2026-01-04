#include "baroToolbar.h"

#include "rc.h"

namespace pizda {
	BaroToolbar::BaroToolbar() {
		auto& settings = RC::getInstance().getSettings();

		// Pressure
		pressure.seven.setDecimalSeparatorIndex(0);
		pressure.seven.setValue(settings.controls.referencePressurePa / 10);
		pressure.setActive(settings.controls.referencePressureSTD);

		pressure.rotated += [this, &settings] {
			settings.controls.referencePressurePa = pressure.seven.getValue() * 10;
			settings.controls.scheduleWrite();
			
			RC::getInstance().getPacketHandler().enqueue(RemotePacketType::baro);
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};

		pressure.pressed += [this, &settings] {
			pressure.setActive(!pressure.isActive());
			
			settings.controls.referencePressureSTD = pressure.isActive();
			settings.controls.scheduleWrite();
			
			RC::getInstance().getPacketHandler().enqueue(RemotePacketType::baro);
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};
		
		row += &pressure;

		// Minimums
		minimums.seven.setValue(settings.controls.minimumAltitudeFt);
		minimums.setActive(settings.controls.minimumAltitudeEnabled);

		minimums.rotated += [this, &settings] {
			settings.controls.minimumAltitudeFt = minimums.seven.getValue();
			settings.controls.scheduleWrite();
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};

		minimums.pressed += [this, &settings] {
			minimums.setActive(!minimums.isActive());
			
			settings.controls.minimumAltitudeFt = minimums.isActive();
			settings.controls.scheduleWrite();
			
			RC::getInstance().getAudioPlayer().playFeedback();
		};
		
		row += &minimums;
	}
}