#include "pressureControls.h"
#include "../../../../../rc.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

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

		pressure.switched += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.controls.referencePressureSTD = pressure.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		row += &pressureTitle;

		// Minimums
		minimums.seven.setValue(settings.controls.minimumAltitudeFt);
		minimums.button.setChecked(settings.controls.minimumAltitudeEnabled);

		minimums.rotated += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.controls.minimumAltitudeFt = minimums.seven.getValue();
			RC::getInstance().getSettings().enqueueWrite();
		};

		minimums.switched += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.controls.minimumAltitudeEnabled = minimums.button.isChecked();
			RC::getInstance().getSettings().enqueueWrite();
		};

		row += &minimumsTitle;

		row.setOrientation(Orientation::horizontal);
		row.setSpacing(10);
		row.setHorizontalAlignment(Alignment::center);
		row.setMargin(Margin(0, 2, 0, 0));
		*this += &row;
	}
}