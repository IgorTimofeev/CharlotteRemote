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

	class MFDLayout : public RelativeStackLayout {
		public:
			void deleteShit();
			void createShit();
			void updateSplitRatio();

			std::unique_ptr<PFD> _PFD {};
			std::unique_ptr<ND> _ND {};
	};

	class MFDPage : public Page {
		public:
			explicit MFDPage();
			~MFDPage() override;

			static MFDPage* getInstance();

			void fromSettings();
			MFDLayout& getMFDLayout();

		private:
			static MFDPage* _instance;

			RelativeStackLayout _rows {};

			MFDLayout _MFDLayout {};

			MainToolbar _mainToolbar {};
			std::unique_ptr<AutopilotToolbar> _autopilotToolbar {};
			std::unique_ptr<BaroToolbar> _baroToolbar {};
			std::unique_ptr<TrimToolbar> _trimToolbar {};
			std::unique_ptr<LightsToolbar> _lightsToolbar {};
	};
}