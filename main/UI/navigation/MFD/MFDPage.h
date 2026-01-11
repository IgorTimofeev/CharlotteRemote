#pragma once

#include <memory>

#include "UI/navigation/page.h"

#include "PFD/PFD.h"
#include "ND/ND.h"
#include "toolbar/main/mainToolbar.h"
#include "toolbar/autopilot/autopilotToolbar.h"
#include "toolbar/baro/baroToolbar.h"
#include "toolbar/trim/trimToolbar.h"
#include "toolbar/lights/lightsToolbar.h"

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

			MainToolbar _mainToolbar {};
			AutopilotToolbar* _autopilotToolbar = nullptr;
			BaroToolbar* _baroToolbar = nullptr;
			TrimToolbar* _trimToolbar = nullptr;
			LightsToolbar* _lightsToolbar = nullptr;

			void fromSettingsInstance();
	};
}