#pragma once

#include "NDScene.h"

#include "UI/theme.h"
#include "UI/elements/dialogs/scrollViewDialog.h"

namespace pizda {
	using namespace YOBA;

	class NDActionButtonDialog : public ScrollViewDialog {
		public:
			NDActionButtonDialog(NDScene* scene);

		private:
			Button _newWaypointButton {};
			Button _setHomeButton {};
	};

	class ND : public Layout {
		public:
			ND();

			NDScene scene {};

		private:

			StackLayout _buttonRows {};

			Button _splitButton {};
			Button _viewModeButton {};
			Button _latLongButton {};
			Button _actionButton {};

			void updateViewModeButtonText();
			void updateSplitButtonText();
			void addGovnoButton(Button* button, const std::function<void()>& onClick);
	};
}
