#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/separator.h"

namespace pizda {
	using namespace YOBA;

	class ADIRSSettingsPage : public ScrollViewPage {
		public:
			ADIRSSettingsPage();

		protected:
			void onTick() override;
		
		private:
			// ----------------------------- ADIRS -----------------------------

			TextField _ADIRSMagneticDeclinationTextField {};
			Titler _ADIRSMagneticDeclinationTitle { L"Magnetic declination (deg)", &_ADIRSMagneticDeclinationTextField };

			// ----------------------------- Calibration -----------------------------

			HorizontalSeparator _calSeparator {};

			TextView _calTitle {};

			TextView _calDescriptionText {};

			ProgressBar _calProgressBar {};
			TextView _calProgressText {};
			
			Button _calAccelAndGyroButton {};
			Button _calMagButton {};
	};
}