#pragma once

#include <memory>

#include "../page.h"

#include "PFD/PFD.h"
#include "ND/ND.h"
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

			std::unique_ptr<PFD> _PFD {};
			std::unique_ptr<ND> _ND {};

			std::unique_ptr<MainControls> _mainControls {};
			std::unique_ptr<AutopilotControls> _autopilotControls {};
			std::unique_ptr<PressureControls> _pressureControls {};

			void fromSettingsInstance();
	};
}