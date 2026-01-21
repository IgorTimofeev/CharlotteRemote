#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/elements/titler.h"
#include "UI/elements/dialogs/scrollViewDialog.h"

#include "types/navigationData.h"
#include "UI/elements/tabSelector.h"

#include "waypointButton.h"

namespace pizda {
	using namespace YOBA;

	class WaypointDialogSelectedItem : public NavigationWaypointDataIndexAware {
		public:
			WaypointDialogSelectedItem(const uint16_t waypointIndex) : NavigationWaypointDataIndexAware(waypointIndex) {

			}
	};

	class SelectWaypointDialog : public ScrollViewDialog {
		public:
			static void select(
				std::wstring_view titleText,
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
				const std::optional<WaypointDialogSelectedItem>& selectedItem,
				const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
			);

			TextField _searchTextField {};
			Titler _searchTextFieldTitle { L"Search", &_searchTextField };

			WaypointButton _waypointButton {};
			Titler _waypointTitle = { L"Waypoint", &_waypointButton };

			Button _confirmButton {};

			void updateVisualsFromWaypoint(uint16_t waypointIndex, const NavigationWaypointData& waypointData);
	};
}
