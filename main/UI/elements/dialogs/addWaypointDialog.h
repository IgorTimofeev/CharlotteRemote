#pragma once

#include <esp_log.h>
#include "cancelConfirmDialog.h"
#include <UI/theme.h>
#include <UI/elements/titler.h>
#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;

	class AddWaypointDialog : public CancelConfirmDialog {
		public:
			TextField name {};
			Titler nameTitle = { L"Name", &name };

			RelativeStackLayout _latitudeAndLongitudeRow { Orientation::horizontal, 10 };

			TextField latitude {};
			Titler latitudeTitle = { L"Latitude", &latitude };

			TextField longitude {};
			Titler longitudeTitle = { L"Longitude", &longitude };

			static void create(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) {
				const auto dialog = new AddWaypointDialog(coordinates, onConfirm);
				dialog->show();
			}

		private:
			std::function<void()> _onConfirm;

			explicit AddWaypointDialog(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) : _onConfirm(onConfirm) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				title.setText(L"Create waypoint");

				// Name
				Theme::apply(&name);
				rows += &nameTitle;

				// Latitude & longitude
				rows += &_latitudeAndLongitudeRow;

				// Latitude
				Theme::apply(&latitude);
				latitude.setText(std::to_wstring(toDegrees(coordinates.getLatitude())));
				_latitudeAndLongitudeRow += &latitudeTitle;

				// Longitude
				Theme::apply(&longitude);
				longitude.setText(std::to_wstring(toDegrees(coordinates.getLongitude())));
				_latitudeAndLongitudeRow += &longitudeTitle;

				// Bottom buttons
				rows += &bottomButtonsRow;

				// Callbacks
				cancelButton.click += [this, &rc] {
					rc.getApplication().scheduleOnTick([this] {
						hide();
						delete this;
					});
				};

				confirmButton.click += [this, &nd, &rc] {
					if (name.getText().size() == 0) {
						name.setBorderColor(&Theme::bad2);
						return;
					}

					rc.getApplication().scheduleOnTick([this, &nd] {
						const auto latitudeRad = toRadians(std::wcstof(latitude.getText().data(), nullptr));
						const auto longitudeRad = toRadians(std::wcstof(longitude.getText().data(), nullptr));

						nd.addRNAVWaypoint(
							NavigationWaypointType::enroute,
							name.getText(),
							GeographicCoordinates(latitudeRad, longitudeRad, 0)
						);

						_onConfirm();

						hide();
						delete this;
					});
				};
			}
	};
}