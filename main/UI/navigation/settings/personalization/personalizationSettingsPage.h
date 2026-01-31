#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/switcher.h"
#include "UI/elements/separator.h"

namespace pizda {
	using namespace YOBA;

	class PersonalizationSettingsPage : public ScrollViewPage {
		public:
			PersonalizationSettingsPage();

		private:
			// -------------------------------- General --------------------------------
			
			Switcher _generalAudioFeedback { L"Audio feedback" };
			Switcher _generalDebugOverlay { L"Debug overlay" };
			Switcher _generalDataInterpolation { L"Data interpolation" };
			HorizontalSeparator _generalSeparator {};

			// -------------------------------- PFD --------------------------------

			TextView _PFDTitle {};

			// Split
			constexpr static uint8_t _PFDSplitMin = 20;
			constexpr static uint8_t _PFDSplitMax = 80;

			Slider _PFDSplitSlider {};
			Titler _PFDSplitTitle = Titler(L"Split view height", &_PFDSplitSlider);

			// FOV
			constexpr static uint8_t _PFDFOVMin = 20;
			constexpr static uint8_t _PFDFOVMax = _PFDFOVMin + 100;

			Slider _PFDFOVSlider {};
			Titler _PFDFOVTitle = Titler(L"Field of view", &_PFDFOVSlider);

			// Waypoint labels
			Switcher _PFDWaypointLabels { L"Waypoint labels" };

			HorizontalSeparator _PFDSeparator {};

			// -------------------------------- ND --------------------------------

			TextView _NDTitle {};

			// Earth grid
			Switcher _NDEarthGrid { L"Earth grid" };
	};
}