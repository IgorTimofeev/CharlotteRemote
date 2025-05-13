#pragma once

#include "NDScene.h"

#include <esp_log.h>

#include "../../../theme.h"
#include "../../../elements/dialog.h"
#include "../../../elements/titler.h"

namespace pizda {
	using namespace YOBA;

	class WaypointDialog : public Dialog {
		public:
			WaypointDialog() {
				title.setText(L"Add waypoint");

				// Name
				Theme::apply(&name);
				rows += &nameTitle;

				// Latitude & longitude
				rows += &_latitudeAndLongitudeRow;

				// Latitude
				Theme::apply(&latitude);
				_latitudeAndLongitudeRow += &latitudeTitle;

				// Longitude
				Theme::apply(&longitude);
				_latitudeAndLongitudeRow += &longitudeTitle;

				// Bottom buttons
				rows += &bottomButtonsRow;

				// Cancel
				Theme::apply(&cancelButton);
				cancelButton.setDefaultBackgroundColor(&Theme::bg3);
				cancelButton.setDefaultTextColor(&Theme::fg3);
				cancelButton.setText(L"Cancel");
				bottomButtonsRow += &cancelButton;

				// Confirm
				Theme::apply(&confirmButton);
				confirmButton.setText(L"Confirm");
				bottomButtonsRow += &confirmButton;
			}

			TextField name {};
			Titler nameTitle = { L"Name", &name };

			RelativeStackLayout _latitudeAndLongitudeRow { Orientation::horizontal, 10 };

			TextField latitude {};
			Titler latitudeTitle = { L"Latitude", &latitude };

			TextField longitude {};
			Titler longitudeTitle = { L"Longitude", &longitude };

			RelativeStackLayout bottomButtonsRow { Orientation::horizontal, 10 };
			Button cancelButton {};
			Button confirmButton {};
	};

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