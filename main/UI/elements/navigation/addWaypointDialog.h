#pragma once

#include <esp_log.h>
#include <UI/elements/dialogs/dialog.h>
#include <UI/theme.h>
#include <UI/elements/titler.h>
#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;

	class AddWaypointDialog : public ScrollViewDialog {
		public:
			static void create(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) {
				const auto dialog = new AddWaypointDialog(coordinates, onConfirm);
				dialog->show();
			}

		private:
			std::function<void()> _onConfirm;

			TabSelectorItem _airportTypeItem {};
			TabSelectorItem _RNAVTypeItem {};
			TabSelectorItem _terminalTypeItem {};
			TabSelector _typeSelector {};

			TextField _nameTextField {};
			Titler _nameTitle = { L"Name", &_nameTextField };

			RelativeStackLayout _latitudeAndLongitudeRow { Orientation::horizontal, 10 };

			TextField _latitude {};
			Titler _latitudeTitle = { L"Latitude", &_latitude };

			TextField _longitude {};
			Titler _longitudeTitle = { L"Longitude", &_longitude };

			Button _confirmButton {};

			explicit AddWaypointDialog(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) : _onConfirm(onConfirm) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				title.setText(L"Create waypoint");

				// Type
				_typeSelector.addItem(&_RNAVTypeItem);
				_typeSelector.addItem(&_terminalTypeItem);
				_typeSelector.addItem(&_airportTypeItem);

				_typeSelector.selectionChanged += [] {

				};

				_typeSelector.setSelectedIndex(0);
				rows += &_typeSelector;

				// Name
				Theme::apply(&_nameTextField);
				rows += &_nameTitle;

				// Latitude & longitude
				rows += &_latitudeAndLongitudeRow;

				// Latitude
				Theme::apply(&_latitude);
				_latitude.setText(std::to_wstring(toDegrees(coordinates.getLatitude())));
				_latitudeAndLongitudeRow += &_latitudeTitle;

				// Longitude
				Theme::apply(&_longitude);
				_longitude.setText(std::to_wstring(toDegrees(coordinates.getLongitude())));
				_latitudeAndLongitudeRow += &_longitudeTitle;

				// Confirm
				Theme::applyPrimary(&_confirmButton);
				_confirmButton.setText(L"Confirm");

				_confirmButton.click += [this, &nd, &rc] {
					if (_nameTextField.getText().size() == 0) {
						_nameTextField.setDefaultBorderColor(&Theme::bad2);
						return;
					}

					rc.getApplication().scheduleOnTick([this, &nd] {
						const auto latitudeRad = toRadians(std::wcstof(_latitude.getText().data(), nullptr));
						const auto longitudeRad = toRadians(std::wcstof(_longitude.getText().data(), nullptr));

						nd.addRNAVWaypoint(
							NavigationWaypointType::enroute,
							_nameTextField.getText(),
							GeographicCoordinates(latitudeRad, longitudeRad, 0)
						);

						_onConfirm();

						hide();
						delete this;
					});
				};

				rows += &_confirmButton;
			}
	};
}