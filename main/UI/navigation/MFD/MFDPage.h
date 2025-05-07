#pragma once

#include "../page.h"

#include "PFD.h"

#include "main/mainControls.h"
#include "ND/NDControls.h"
#include "autopilot/autopilotControls.h"
#include "pressure/pressureControls.h"

namespace pizda {
	using namespace YOBA;

	class MFDPage : public Page {
		public:
			explicit MFDPage();

			~MFDPage() override;

			static void fromSettings();

		private:
			static MFDPage* _instance;

			PFD _PFD {};

			RelativeStackLayout _rows {};

			NDControls* _NDControls = nullptr;
			MainControls* _mainControls = nullptr;
			AutopilotControls* _autopilotControls = nullptr;
			PressureControls* _pressureControls = nullptr;

			void fromSettingsInstance();
	};
}