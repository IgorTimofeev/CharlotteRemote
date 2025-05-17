#pragma once

#include "NDScene.h"

#include <esp_log.h>

#include <UI/theme.h>

namespace pizda {
	using namespace YOBA;

	class ND : public Layout {
		public:
			ND();

			void updateViewModeButtonText();

		private:
			NDScene _scene {};

			StackLayout _rightRows {};

			Button _viewModeButton {};
			Button _latLongButton {};
			Button _waypointButton {};

			void addGovnoButton(Button* button, const std::function<void()>& onClick);
	};
}