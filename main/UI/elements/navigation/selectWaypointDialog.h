#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/titler.h>
#include <UI/elements/dialogs/dialog.h>

#include <types/navigationData.h>
#include <UI/elements/tabSelector.h>

#include "waypointButton.h"

namespace pizda {
	using namespace YOBA;

	class RunwayItem : public SelectorItem {
		public:
			explicit RunwayItem(const NavigationAirportAndRunwayIndicesData& airportAndRunway);

			NavigationAirportAndRunwayIndicesData airportAndRunway;

		protected:
			void onRender(Renderer* renderer) override;
	};

	class WaypointDialogSelectedItem : public NavigationWaypointDataIndexAware {
		public:
			WaypointDialogSelectedItem(uint16_t waypointIndex, const std::optional<NavigationAirportAndRunwayIndicesData>& selectedItem) :
				NavigationWaypointDataIndexAware(waypointIndex),
				airportAndRunway(selectedItem)
			{

			}

			std::optional<NavigationAirportAndRunwayIndicesData> airportAndRunway;
	};

	class SelectWaypointDialog : public Dialog {
		public:
			static void select(
				std::wstring_view titleText,
				bool airportsOnly,
				const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
			) {
				(new SelectWaypointDialog(titleText, airportsOnly, std::nullopt, onConfirm))->show();
			}

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
