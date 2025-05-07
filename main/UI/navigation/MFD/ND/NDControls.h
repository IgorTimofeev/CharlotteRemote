#pragma once

#include "ND.h"

#include <esp_log.h>

#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;

	class NDControls : public Layout {
		public:
			NDControls();

			void updateOrientationButtonText();

		private:
			ND _ND {};

			Button _orientationButton {};
			StackLayout _rightRows {};
			Button _latLongButton {};

			void addGovnoButton(Button* button, const std::function<void()>& onClick);
	};
}