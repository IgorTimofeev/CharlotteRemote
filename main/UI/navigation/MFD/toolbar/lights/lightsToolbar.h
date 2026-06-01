#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/toolbarButton.h"
#include "UI/navigation/MFD/toolbar/rotaryControl.h"
#include "resources/images.h"

namespace pizda {
	using namespace YOBA;
	
	class LightsToolbar : public RowToolbar {
		public:
			explicit LightsToolbar();
		
		protected:
			void onTick() override;
			
		private:
			ImageToolbarButton navigation { "Nav", &resources::images::menuIconMFDLightsNavigation };
			ImageToolbarButton strobe { "Strobe", &resources::images::menuIconMFDLightsStrobe };
			ImageToolbarButton landing { "Landing", &resources::images::menuIconMFDLightsLanding };
			ImageToolbarButton cabin { "Cabin", &resources::images::menuIconMFDLightsCabin };
			
			void copyAircraftToRemote();
	};
}