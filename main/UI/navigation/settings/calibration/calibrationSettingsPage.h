#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"

namespace pizda {
	using namespace YOBA;

	class CalibrationSettingsPage : public ScrollViewPage {
		public:
			CalibrationSettingsPage();

		protected:
			void onTick() override;
		
		private:
			Text _descriptionText {};
			
			ProgressBar _progressBar {};
			Text _progressText {};
			
			Button _accelAndGyroButton {};
			Button _magButton {};
	};
}