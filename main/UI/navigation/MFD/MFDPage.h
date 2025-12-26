#pragma once

#include <memory>

#include <UI/navigation/page.h>

#include "PFD/PFD.h"
#include "ND/ND.h"
#include "toolbar/main/mainControls.h"
#include "toolbar/autopilot/autopilotControls.h"
#include "toolbar/baro/baroControls.h"

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
			ND* _ND = nullptr;

			MainControls _mainControls {};
			
			AutopilotControls* _autopilotControls = nullptr;
			BaroControls* _baroControls = nullptr;

			void fromSettingsInstance();
	};
}