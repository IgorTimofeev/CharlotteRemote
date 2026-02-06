#pragma once

#include "NDScene.h"

#include "UI/theme.h"

namespace pizda {
	using namespace YOBA;

	class ND : public Layout {
		public:
			ND();

		private:
			NDScene _scene {};

			StackLayout _buttonRows {};

			Button _viewModeButton {};
			Button _latLongButton {};
			Button _waypointButton {};
			Button _splitButton {};

			void updateViewModeButtonText();
			void updateSplitButtonText();
			void addGovnoButton(Button* button, const std::function<void()>& onClick);
	};
}