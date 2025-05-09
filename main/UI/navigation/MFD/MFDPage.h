#pragma once

#include "../page.h"

#include "PFD/PFD.h"
#include "ND/NDControls.h"
#include "flightPlan/flightPlan.h"
#include "toolbar/main/mainControls.h"
#include "toolbar/autopilot/autopilotControls.h"
#include "toolbar/pressure/pressureControls.h"

namespace pizda {
	using namespace YOBA;

	class MFDPage : public Page {
		public:
			explicit MFDPage();

			~MFDPage() override;

			static void fromSettings();

		private:
			static MFDPage* _instance;

			RelativeStackLayout _rows {};

			PFD* _PFD = nullptr;
			NDControls* _NDControls = nullptr;
			FlightPlan* _flightPlan = nullptr;
			MainControls* _mainControls = nullptr;
			AutopilotControls* _autopilotControls = nullptr;
			PressureControls* _pressureControls = nullptr;

			void fromSettingsInstance();
	};
}