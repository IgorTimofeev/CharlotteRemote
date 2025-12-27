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
	
	class LightsToolbar : public Toolbar {
		public:
			explicit LightsToolbar();
			
			KorryButton2 navigation { L"Nav", &resources::Images::menuIconMFDLightsNavigation };
			KorryButton2 strobe { L"Strobe", &resources::Images::menuIconMFDLightsStrobe };
			KorryButton2 landing { L"Landing", &resources::Images::menuIconMFDLightsLanding };
		
		private:

	};
}