#include "pressureControls.h"
#include "../../../../rc.h"

namespace pizda {
	using namespace YOBA;

	PressureControls::PressureControls() {
		setHeight(56);

		auto& settings = RC::getInstance().getSettings();

		// Pressure
		pressure.seven.setValue(settings.controls.referencePressurePa / 100);
		pressure.button.setChecked(settings.controls.referencePressureSTD);

		pressure.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.controls.referencePressurePa = pressure.seven.getValue() * 100;
			RC::getInstance().getSettings().enqueueWrite();
		};

		pressure.button.isCheckedChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.controls.referencePressureSTD = pressure.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		applyTitlerStyle(pressureTitle);
		row += &pressureTitle;

		// Minimums
		minimums.seven.setValue(settings.controls.minimumAltitudeFt);
		minimums.button.setChecked(settings.controls.minimumAltitudeEnabled);

		minimums.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.controls.minimumAltitudeFt = minimums.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		minimums.button.isCheckedChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.controls.minimumAltitudeEnabled = minimums.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		applyTitlerStyle(minimumsTitle);
		row += &minimumsTitle;
	}
}