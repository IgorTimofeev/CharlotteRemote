#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <esp_log.h>
#include <UI/elements/dialogs/scrollViewDialog.h>
#include "UI/theme.h"
#include <UI/elements/titler.h>
#include <UI/elements/tabSelector.h>
#include <types/navigationData.h>

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

			void fromRadians(float lat, float lon) {
				latitude.setText(std::to_wstring(toDegrees(lat)));
				longitude.setText(std::to_wstring(toDegrees(lon)));
			}

			void toRadians(float& lat, float& lon) const {
				lat = YOBA::toRadians(std::wcstof(latitude.getText().data(), nullptr));
				lon = YOBA::toRadians(std::wcstof(longitude.getText().data(), nullptr));
			}
	};

	class AddWaypointDialogRunway : public Layout {
		public:
			AddWaypointDialogRunway() {
				backgroundRectangle.setFillColor(&Theme::bg1);
				backgroundRectangle.setCornerRadius(Theme::cornerRadius);
				*this += &backgroundRectangle;

				// Heading & alignment
				headingAndAlignmentRow.setOrientation(Orientation::horizontal);
				headingAndAlignmentRow.setSpacing(Theme::spacing);

				// Heading
				Theme::apply(&headingTextField);
				headingAndAlignmentRow += &headingTextField;

				// Alignment
				noneAlignmentItem.setText(L"-");
				alignmentSelector.addItem(&noneAlignmentItem);

				leftAlignmentItem.setText(L"L");
				alignmentSelector.addItem(&leftAlignmentItem);

				centerAlignmentItem.setText(L"C");
				alignmentSelector.addItem(&centerAlignmentItem);

				rightAlignmentItem.setText(L"R");
				alignmentSelector.addItem(&rightAlignmentItem);

				alignmentSelector.setSelectedIndex(0);

				alignmentSelector.applyDialogStyle();
				headingAndAlignmentRow += &alignmentSelector;

				rows += &headingAndAlignmentTitle;

				// Latitude & longitude
				rows += &latLonTitle;

				// Length & width
				lengthAndWidthRow.setOrientation(Orientation::horizontal);
				lengthAndWidthRow.setSpacing(Theme::spacing);

				Theme::apply(&lengthTextField);
				lengthAndWidthRow += &lengthTextField;

				Theme::apply(&widthTextField);
				lengthAndWidthRow += &widthTextField;

				rows += &lengthAndWidthTitle;

				// Rows
				rows.setSpacing(Theme::spacing);
				rows.setMargin(Margin(10));
				*this += &rows;

				// Remove button
				Theme::applyCritical(&removeButton);
				removeButton.setCornerRadius(2);
				removeButton.setSize(Size(14, 14));
				removeButton.setAlignment(Alignment::end, Alignment::start);
				removeButton.setMargin(Margin(0, 5, 5, 0));
				removeButton.setText(L"X");

				removeButton.click += [this] {
					RC::getInstance().getApplication().scheduleOnTick([this] {
						getParent()->removeChild(this);
						delete this;
					});
				};

				*this += &removeButton;
			}

		private:
			Rectangle backgroundRectangle {};

			StackLayout rows {};

			LatLonRow latLon {};
			Titler latLonTitle { L"Latitude & longitude", &latLon };

			RelativeStackLayout headingAndAlignmentRow {};

			TextField headingTextField {};

			TabSelectorItem noneAlignmentItem {};
			TabSelectorItem leftAlignmentItem {};
			TabSelectorItem centerAlignmentItem {};
			TabSelectorItem rightAlignmentItem {};
			TabSelector alignmentSelector {};

			Titler headingAndAlignmentTitle { L"Heading & alignment", &headingAndAlignmentRow };

			RelativeStackLayout lengthAndWidthRow {};
			TextField lengthTextField {};
			TextField widthTextField {};
			Titler lengthAndWidthTitle { L"Length & width", &lengthAndWidthRow };

			Button removeButton {};
	};

	class AddWaypointDialog : public ScrollViewDialog {
		public:
			~AddWaypointDialog() override {
				while (_runwaysLayout.getChildrenCount() > 1) {
					const auto child = _runwaysLayout[0];
					_runwaysLayout.removeChildAt(0);
					delete child;
				}
			}

			static void create(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) {
				const auto dialog = new AddWaypointDialog(coordinates, onConfirm);
				dialog->show();
			}

		private:
			std::function<void()> _onConfirm;

			TabSelectorItem _routeTypeItem {};
			TabSelectorItem _terminalTypeItem {};
			TabSelectorItem _airportTypeItem {};
			TabSelector _typeSelector {};

			TextField _nameTextField {};
			Titler _nameTitle = { L"Name", &_nameTextField };

			RelativeStackLayout _latitudeAndLongitudeRow { Orientation::horizontal, 10 };

			LatLonRow _latLon {};
			Titler _latLonTitle = { L"Latitude & longitude", &_latLon };

			StackLayout _runwaysLayout {};
			Button _runwaysAddButton {};
			Titler _runwaysTitle { L"Runways", &_runwaysLayout };

			Button _confirmButton {};

			explicit AddWaypointDialog(const GeographicCoordinates& coordinates, const std::function<void()>& onConfirm) : _onConfirm(onConfirm) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				title.setText(L"Create waypoint");

				// Type
				_routeTypeItem.setText(L"Route");
				_typeSelector.addItem(&_routeTypeItem);

				_terminalTypeItem.setText(L"Terminal");
				_typeSelector.addItem(&_terminalTypeItem);

				_airportTypeItem.setText(L"Airport");
				_typeSelector.addItem(&_airportTypeItem);

				_typeSelector.applyDialogStyle();
				_typeSelector.setSelectedIndex(0);

				_typeSelector.selectionChanged += [this] {
					// Airport
					_runwaysTitle.setVisible(_typeSelector.getSelectedIndex() == 2);
				};

				rows += &_typeSelector;

				// Name
				Theme::apply(&_nameTextField);
				rows += &_nameTitle;

				// Latitude & longitude
				_latLon.fromRadians(coordinates.getLatitude(), coordinates.getLongitude());
				rows += &_latLonTitle;

				// Runways
				Theme::applyPlaceholder(&_runwaysAddButton);
				_runwaysAddButton.setText(L"Add runway");

				_runwaysAddButton.click += [this] {
					_runwaysLayout.insertChildFromEnd(1, new AddWaypointDialogRunway());
				};

				_runwaysLayout.setSpacing(Theme::spacing);
				_runwaysLayout += &_runwaysAddButton;

				_runwaysTitle.setVisible(false);
				rows += &_runwaysTitle;

				// Confirm
				Theme::applyPrimary(&_confirmButton);
				_confirmButton.setText(L"Confirm");
				// _confirmButton.setCornerRadius(0);
				// _confirmButton.setMargin(Margin(-15, 0, -15, -15));

				_confirmButton.click += [this, &nd, &rc] {
					if (_nameTextField.getText().size() == 0) {
						_nameTextField.setDefaultBorderColor(&Theme::bad2);
						return;
					}

					rc.getApplication().scheduleOnTick([this, &nd] {
						float latitudeRad, longitudeRad;
						_latLon.toRadians(latitudeRad, longitudeRad);

						nd.addRNAVWaypoint(
							NavigationWaypointType::route,
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