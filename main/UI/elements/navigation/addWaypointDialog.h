#pragma once

#include <esp_log.h>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/elements/dialogs/scrollViewDialog.h"
#include "UI/theme.h"
#include "UI/elements/titler.h"
#include "UI/elements/tabSelector.h"
#include "types/navigationData.h"

namespace pizda {
	using namespace YOBA;

	class LatLonRow : public RelativeStackLayout {
		public:
			LatLonRow() {
				setOrientation(Orientation::horizontal);
				setSpacing(Theme::spacing);

				Theme::apply(&latitude);
				*this += &latitude;

				Theme::apply(&longitude);
				*this += &longitude;
			}

			TextField latitude {};
			TextField longitude {};

			void fromRadians(const float lat, const float lon) {
				latitude.setText(std::to_wstring(toDegrees(lat)));
				longitude.setText(std::to_wstring(toDegrees(lon)));
			}

			void toRadians(float& lat, float& lon) const {
				lat = YOBA::toRadians(std::wcstof(latitude.getText().data(), nullptr));
				lon = YOBA::toRadians(std::wcstof(longitude.getText().data(), nullptr));
			}
	};

	class AddWaypointDialog : public ScrollViewDialog {
		public:
			static void create(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) {
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

			explicit AddWaypointDialog(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) : _onConfirm(onConfirm) {
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
				// _confirmButton.setCornerRadius(0);
				// _confirmButton.setMargin(Margin(-15, 0, -15, -15));

				_confirmButton.click += [this, &nd, &rc] {
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