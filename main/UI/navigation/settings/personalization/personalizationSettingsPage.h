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
			
			Switcher _audioFeedback { L"Audio feedback" };
			Switcher _debugOverlay { L"Debug overlay" };
			Separator _generalSeparator {};
			
			// -------------------------------- MFD --------------------------------
			
			Text _MFDTitle {};
			
			// Split
			Slider _MFDSplitSlider {};
			Titler _MFDSplitTitle = Titler(&_MFDSplitSlider);
			
			constexpr static uint8_t _MFDSplitMin = 20;
			constexpr static uint8_t _MFDSplitMax = 80;
			
			void updateMFDHeightTitle();
			
			// FOV
			Slider _MFDFOVSlider {};
			Titler _MFDFOVTitle = Titler(&_MFDFOVSlider);
			
			constexpr static uint8_t _MFDFOVMin = 20;
			constexpr static uint8_t _MFDFOVMax = 120;
			
			void updateMFDFOVTitle();
			
			// LPF
			Switcher _MFDLPF { L"Low pass filter" };
			
			// Earth grid
			Switcher _MFDSphereSwitcher { L"ND earth grid" };
	};
}