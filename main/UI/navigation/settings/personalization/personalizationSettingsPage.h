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
			~PersonalizationSettingsPage() override;

		private:
			static int32_t _scrollPosition;

			// -------------------------------- General --------------------------------
			
			Switcher _generalAudioFeedback { "Audio feedback" };
			Switcher _generalDebugOverlay { "Debug overlay" };
			Switcher _generalDataInterpolation { "Data interpolation" };
			HorizontalSeparator _generalSeparator {};

			// -------------------------------- PFD --------------------------------

			TextView _PFDTitle {};

			// FOV
			Slider _PFDFOVSlider {};
			Titler _PFDFOVTitle = Titler("Field of view", &_PFDFOVSlider);

			// Waypoint labels
			Switcher _PFDWaypointLabels { "Waypoint labels" };

			HorizontalSeparator _PFDSeparator {};

			// -------------------------------- ND --------------------------------

			TextView _NDTitle {};

			// Earth grid
			Switcher _NDEarthGrid { "Earth grid" };
	};
}