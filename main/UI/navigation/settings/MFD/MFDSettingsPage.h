#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../page.h"
#include "../../../elements/titler.h"
#include "../../../elements/switcher.h"

namespace pizda {
	using namespace YOBA;

	class MFDSettingsPage : public ScrollViewPage {
		public:
			MFDSettingsPage();

		private:
			// -------------------------------- MFD --------------------------------

			// Height
			Slider _MFDSplitSlider {};
			Titler _MFDSplitTitle = Titler(&_MFDSplitSlider);

			constexpr static uint8_t _MFDSplitMin = 20;
			constexpr static uint8_t _MFDSplitMax = 80;

			void updateMFDHeightTitle();


			// -------------------------------- PFD --------------------------------

			Text _PFDTitle {};

			// FOV
			Slider _PFDFOVSlider {};
			Titler _PFDFOVTitle = Titler(&_PFDFOVSlider);

			constexpr static uint8_t _PFDFOVMin = 20;
			constexpr static uint8_t _PFDFOVMax = 120;

			void updatePFDFOVTitle();

			// -------------------------------- ND --------------------------------

			Text _NDTitle {};

			// Sphere
			Switcher _MFDSphereSwitcher { L"Show Earth" };
	};
}