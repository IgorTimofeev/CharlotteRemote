#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/titler.h>
#include <UI/elements/dialogs/scrollViewDialog.h>

#include <types/navigationData.h>
#include <UI/elements/tabSelector.h>

#include "waypointButton.h"

namespace pizda {
	using namespace YOBA;

	class RunwayItem : public TabSelectorItem {
		public:
			explicit RunwayItem(const NavigationAirportAndRunwayIndicesData& airportAndRunway);

			NavigationAirportAndRunwayIndicesData airportAndRunway;
	};

	class WaypointDialogSelectedItem : public NavigationWaypointDataIndexAware {
		public:
			WaypointDialogSelectedItem(uint16_t waypointIndex, const std::optional<NavigationAirportAndRunwayIndicesData>& selectedItem);

			std::optional<NavigationAirportAndRunwayIndicesData> airportAndRunway;
	};

	class SelectWaypointDialog : public ScrollViewDialog {
		public:
			static void select(
				std::wstring_view titleText,
				bool airportsOnly,
				const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
			);

			static void edit(
				std::wstring_view titleText,
				const WaypointDialogSelectedItem& selectedItem,
				const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
			);

		private:
			SelectWaypointDialog(
				std::wstring_view titleText,
				bool airportsOnly,
				const std::optional<WaypointDialogSelectedItem>& selectedItem,
				const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
			);

			TextField _searchTextField {};
			Titler _searchTextFieldTitle { L"Search", &_searchTextField };

			WaypointButton _waypointButton {};
			Titler _waypointTitle = { L"Waypoint", &_waypointButton };

			TabSelector _runwaysSelector {};
			Titler _runwaysTitle = { L"Runway", &_runwaysSelector };

			Button _confirmButton {};

			void select(uint16_t waypointIndex, const NavigationWaypointData& waypointData, uint16_t runwayIndex);
	};
}
