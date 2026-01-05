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
			ToolbarButton navigation { L"Nav", &resources::images::menuIconMFDLightsNavigation };
			ToolbarButton strobe { L"Strobe", &resources::images::menuIconMFDLightsStrobe };
			ToolbarButton landing { L"Landing", &resources::images::menuIconMFDLightsLanding };
			ToolbarButton cabin { L"Cabin", &resources::images::menuIconMFDLightsCabin };
	};
}