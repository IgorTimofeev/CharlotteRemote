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

			// FOV
			Slider _PFDFOVSlider {};
			Titler _PFDFOVTitle = Titler(L"Field of view", _PFDFOVSlider);

			// Waypoint labels
			Switcher _PFDWaypointLabels { L"Waypoint labels" };

			HorizontalSeparator _PFDSeparator {};

			// -------------------------------- ND --------------------------------

			TextView _NDTitle {};

			// Earth grid
			Switcher _NDEarthGrid { L"Earth grid" };
	};
}