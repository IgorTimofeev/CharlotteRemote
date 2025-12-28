#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/korryButton.h"
#include "UI/navigation/MFD/toolbar/rotaryControl.h"
#include "resources/images.h"

namespace pizda {
	using namespace YOBA;
	
	class LightsToolbar : public RowToolbar {
		public:
			explicit LightsToolbar();
			
			KorryButton navigation { L"Nav", &resources::images::menuIconMFDLightsNavigation };
			KorryButton strobe { L"Strobe", &resources::images::menuIconMFDLightsStrobe };
			KorryButton landing { L"Landing", &resources::images::menuIconMFDLightsLanding };
			KorryButton cabin { L"Cabin", &resources::images::menuIconMFDLightsCabin };
		
		private:

	};
}