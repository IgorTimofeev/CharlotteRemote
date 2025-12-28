#include "baroToolbar.h"

#include "rc.h"

namespace pizda {
	BaroToolbar::BaroToolbar() {
		setHeight(36);
		
		row.setSpacing(1);
		
		auto& settings = RC::getInstance().getSettings();

		// Pressure
		pressure.seven.setDecimalSeparatorIndex(0);
		pressure.seven.setValue(settings.controls.referencePressurePa / 10);
		pressure.button.setActive(settings.controls.referencePressureSTD);

		pressure.rotated += [this, &settings] {
			settings.controls.referencePressurePa = pressure.seven.getValue() * 10;
			RC::getInstance().getSettings().controls.scheduleWrite();
		};

		pressure.button.isActiveChanged += [this, &settings] {
			settings.controls.referencePressureSTD = pressure.button.isActive();
			RC::getInstance().getSettings().controls.scheduleWrite();
		};
		
		row += &pressure;

		// Minimums
		minimums.seven.setValue(settings.controls.minimumAltitudeFt);
		minimums.button.setActive(settings.controls.minimumAltitudeEnabled);

		minimums.rotated += [this, &settings] {
			settings.controls.minimumAltitudeFt = minimums.seven.getValue();
			RC::getInstance().getSettings().controls.scheduleWrite();
		};

		minimums.button.isActiveChanged += [this, &settings] {
			settings.controls.minimumAltitudeEnabled = minimums.button.isActive();
			RC::getInstance().getSettings().controls.scheduleWrite();
		};
		
		row += &minimums;
	}
}