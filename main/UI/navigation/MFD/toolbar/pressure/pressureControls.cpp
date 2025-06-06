#include "pressureControls.h"

#include <rc.h>

namespace pizda {
	using namespace YOBA;

	PressureControls::PressureControls() {
		setHeight(56);

		auto& settings = RC::getInstance().getSettings();

		row.setSpacing(8);

		// Pressure
		pressure.seven.setValue(settings.controls.referencePressurePa / 100);
		pressure.button.setActive(settings.controls.referencePressureSTD);

		pressure.rotated += [this, &settings] {
			settings.controls.referencePressurePa = pressure.seven.getValue() * 100;
			RC::getInstance().getSettings().controls.scheduleWrite();
		};

		pressure.button.isActiveChanged += [this, &settings] {
			settings.controls.referencePressureSTD = pressure.button.isActive();
			RC::getInstance().getSettings().controls.scheduleWrite();
		};

		pressureLayout.setFocusable(false);
		row += &pressureLayout;

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

		minimumsLayout.setFocusable(false);
		row += &minimumsLayout;
	}
}