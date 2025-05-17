#pragma once

#include <esp_log.h>
#include "cancelConfirmDialog.h"
#include <UI/theme.h>
#include <UI/elements/titler.h>
#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;

	class WaypointDialog : public CancelConfirmDialog {
		public:
			TextField name {};
			Titler nameTitle = { L"Name", &name };

			RelativeStackLayout _latitudeAndLongitudeRow { Orientation::horizontal, 10 };

			TextField latitude {};
			Titler latitudeTitle = { L"Latitude", &latitude };

			TextField longitude {};
			Titler longitudeTitle = { L"Longitude", &longitude };

			static void create(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) {
				const auto dialog = new WaypointDialog(coordinates, onConfirm);
				dialog->show();
			}

		private:
			std::function<void()> _onConfirm;

			explicit WaypointDialog(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) : _onConfirm(onConfirm) {
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
				cancelButton.click += [this] {
					Application::getCurrent()->scheduleTask([this] {
						hide();
						delete this;
					});
				};

				confirmButton.click += [this] {
					if (name.getText().size() == 0) {
						name.setBorderColor(&Theme::bad2);
						return;
					}

					Application::getCurrent()->scheduleTask([this] {
						const auto latitudeRad = toRadians(std::wcstof(latitude.getText().data(), nullptr));
						const auto longitudeRad = toRadians(std::wcstof(longitude.getText().data(), nullptr));

						auto& nd = RC::getInstance().getNavigationData();

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