#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/titler.h>
#include <UI/elements/dialogs/dialog.h>

#include <types/navigationData.h>

#include "waypointButton.h"

namespace pizda {
	using namespace YOBA;

	class RunwayItem : public SelectorItem {
		public:
			explicit RunwayItem(const NavigationAirportIndexAndRunwayIndexData& airportAndRunway);

			NavigationAirportIndexAndRunwayIndexData airportAndRunway;

		protected:
			void onRender(Renderer* renderer) override;
	};

	class SelectWaypointDialog : public Dialog {
		public:
			SelectWaypointDialog(
				std::wstring_view titleText,
				const std::function<bool(const NavigationWaypointData&)>& filter,
				const std::function<void(uint16_t, const std::optional<NavigationAirportIndexAndRunwayIndexData>& airportAndRunway)>& onConfirm
			);

		private:
			TextField _name {};
			Titler _nameTitle { L"Search", &_name };

			WaypointButton _waypointButton {};
			Button _waypointPlaceholder {};
			Layout _waypointLayout {};
			Titler _waypointTitle = { L"Waypoint", &_waypointLayout };

			RelativeStackLayout _runwaysLayout {};
			Selector _runwaysSelector {};
			Titler _runwaysTitle = { L"Runway", &_runwaysSelector };

			Button _confirmButton {};
	};
}
