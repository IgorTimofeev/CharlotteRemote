#pragma once

#include <esp_log.h>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/elements/dialogs/scrollViewDialog.h"
#include "UI/theme.h"
#include "UI/elements/titler.h"
#include "UI/elements/tabSelector.h"
#include "types/navigationData.h"
#include "utilities/string.h"

namespace pizda {
	using namespace YOBA;

	class LatLonRow : public RelativeStackLayout {
		public:
			LatLonRow() {
				setOrientation(Orientation::horizontal);
				setGap(Theme::gap);

				Theme::apply(&latitude);
				*this += &latitude;

				Theme::apply(&longitude);
				*this += &longitude;
			}

			TextField latitude {};
			TextField longitude {};

			void fromRadians(const float lat, const float lon) {
				latitude.setText(StringUtils::toWString(toDegrees(lat)));
				longitude.setText(StringUtils::toWString(toDegrees(lon)));
			}

			void toRadians(float& lat, float& lon) const {
				if (!StringUtils::tryParseFloat(latitude.getText().data(), lat))
					lat = 0;

				if (!StringUtils::tryParseFloat(longitude.getText().data(), lon))
					lon = 0;
			}
	};

	class AddWaypointDialog : public ScrollViewDialog {
		public:
			static void create(const GeoCoordinates& coordinates, const std::function<void()>& onConfirm) {
				const auto dialog = new AddWaypointDialog(coordinates, onConfirm);
				dialog->show();
			}

		private:
			std::function<void()> _onConfirm;

			TabSelectorItem _enrouteTypeItem {};
			TabSelectorItem _runwayTypeItem {};
			TabSelector _typeSelector {};

			TextField _nameTextField {};
			Titler _nameTitle = { L"Name", &_nameTextField };

			RelativeStackLayout _latitudeAndLongitudeRow { Orientation::horizontal, 10 };

			LatLonRow _latLon {};
			Titler _latLonTitle = { L"Latitude & longitude", &_latLon };

			Button _confirmButton {};

			explicit AddWaypointDialog(const GeoCoordinates& coordinates, const std::function<void()>& onConfirm) : _onConfirm(onConfirm) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				title.setText(L"Create waypoint");

				// Type
				_enrouteTypeItem.setText(L"Enroute");
				_typeSelector.addItem(&_enrouteTypeItem);

				_runwayTypeItem.setText(L"Runway");
				_typeSelector.addItem(&_runwayTypeItem);

				_typeSelector.applyDialogStyle();
				_typeSelector.setSelectedIndex(0);

				rows += &_typeSelector;

				// Name
				Theme::apply(&_nameTextField);
				rows += &_nameTitle;

				// Latitude & longitude
				_latLon.fromRadians(coordinates.getLatitude(), coordinates.getLongitude());
				rows += &_latLonTitle;

				// Confirm
				Theme::applyPrimary(&_confirmButton);
				_confirmButton.setText(L"Confirm");

				_confirmButton.setOnClick([this, &nd, &rc] {
					if (_nameTextField.getText().size() == 0) {
						_nameTextField.setDefaultBorderColor(&Theme::bad1);
						return;
					}

					rc.getApplication().scheduleOnTick([this, &nd] {
						float latitudeRad, longitudeRad;
						_latLon.toRadians(latitudeRad, longitudeRad);

						nd.addEnrouteWaypoint(
							NavigationWaypointType::enroute,
							_nameTextField.getText(),
							GeoCoordinates(latitudeRad, longitudeRad, 0)
						);

						_onConfirm();

						hide();
						delete this;
					});
				});

				rows += &_confirmButton;
			}
	};
}