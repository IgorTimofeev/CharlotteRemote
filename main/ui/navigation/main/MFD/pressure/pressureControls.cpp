#include "pressureControls.h"
#include "../../../../../rc.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	PressureControls::PressureControls() {
		setHeight(48);

		row.setOrientation(Orientation::horizontal);
		row.setSpacing(10);

		auto& settings = RC::getInstance().getSettings();

		pressure.addSettingsCallbacks(&settings.controls.referencePressurePa, &settings.controls.referencePressureSTD);
		row += &pressureTitle;

		minimums.addSettingsCallbacks(&settings.controls.minimumAltitudeFt, &settings.controls.minimumAltitudeEnabled);
		row += &minimumsTitle;

		*this += &row;
	}
}