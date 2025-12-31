#include "baroToolbar.h"

#include "rc.h"

namespace pizda {
	BaroToolbar::BaroToolbar() {
		setHeight(36);
		
		auto& settings = RC::getInstance().getSettings();

		// Pressure
		pressure.seven.setDecimalSeparatorIndex(0);
		pressure.seven.setValue(settings.controls.referencePressurePa / 10);
		pressure.setActive(settings.controls.referencePressureSTD);

		pressure.rotated += [this, &settings] {
			settings.controls.referencePressurePa = pressure.seven.getValue() * 10;
			RC::getInstance().getSettings().controls.scheduleWrite();
		};

		pressure.isActiveChanged += [this, &settings] {
			settings.controls.referencePressureSTD = pressure.isActive();
			RC::getInstance().getSettings().controls.scheduleWrite();
		};
		
		row += &pressure;

		// Minimums
		minimums.seven.setValue(settings.controls.minimumAltitudeFt);
		minimums.setActive(settings.controls.minimumAltitudeEnabled);

		minimums.rotated += [this, &settings] {
			settings.controls.minimumAltitudeFt = minimums.seven.getValue();
			RC::getInstance().getSettings().controls.scheduleWrite();
		};

		minimums.isActiveChanged += [this, &settings] {
			settings.controls.minimumAltitudeEnabled = minimums.isActive();
			RC::getInstance().getSettings().controls.scheduleWrite();
		};
		
		row += &minimums;
	}
}