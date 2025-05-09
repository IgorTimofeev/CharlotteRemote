#pragma once

#include "ND.h"

#include <esp_log.h>

#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;

	class NDControls : public Layout {
		public:
			NDControls();

			void updateViewModeButtonText();

		private:
			ND _ND {};

			Button _viewModeButton {};
			StackLayout _rightRows {};
			Button _latLongButton {};

			void addGovnoButton(Button* button, const std::function<void()>& onClick);
	};
}