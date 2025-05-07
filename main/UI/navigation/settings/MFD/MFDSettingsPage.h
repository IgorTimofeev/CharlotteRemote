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
			Slider _PFDFOVSlider {};
			Titler _PFDFOVTitle = Titler(&_PFDFOVSlider);

			constexpr static const uint8_t _PFDFOVMin = 20;
			constexpr static const uint8_t _PFDFOVMax = 120;

			Text _MFDTitle {};

			Slider _MFDHeightSlider {};
			Titler _MFDHeightTitle = Titler(&_MFDHeightSlider);

			constexpr static const uint8_t _MFDHeightMin = 20;
			constexpr static const uint8_t _MFDHeightMax = 80;

			Switcher _MFDSphereSwitcher { L"Show earth bounds" };

			void updatePFDFOVTitle();
			void updateMFDHeightTitle();
	};
}