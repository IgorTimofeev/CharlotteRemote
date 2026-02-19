#pragma once

#include "settings/controlSettings.h"
#include "settings/axesSettings.h"
#include "settings/autopilotSettings.h"
#include "settings/personalizationSettings.h"
#include "settings/navigationSettings.h"
#include "settings/motorsSettings.h"
#include "settings/transceiverSettings.h"
#include "settings/ADIRSSettings.h"
#include "settings/PIDSettings.h"

namespace pizda {
	class Settings {
		public:
			ADIRSSettings ADIRS {};
			ControlSettings controls {};
			AxesSettings axes {};
			AutopilotSettings autopilot {};
			PersonalizationSettings personalization {};
			NavigationSettings navigation {};
			MotorsSettings motors {};
			TransceiverSettings transceiver {};
			PIDSettings PID {};

			void readAll() {
				ADIRS.read();
				controls.read();
				axes.read();
				autopilot.read();
				personalization.read();
				navigation.read();
				motors.read();
				transceiver.read();
				PID.read();
			}
	};
}
