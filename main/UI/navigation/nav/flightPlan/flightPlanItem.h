#pragma once

#include <format>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>
#include <types/navigationData.h>
#include <utils/rendering.h>
#include <UI/elements/navigation/waypointButton.h>

namespace pizda {
	using namespace YOBA;

	class FlightPlanItem : public WaypointButton {
		public:
			explicit FlightPlanItem();

		protected:
			void onRender(Renderer* renderer) override;
	};

	class AirportFlightPlanItem : public FlightPlanItem {
		public:
			explicit AirportFlightPlanItem(bool destination);

			const std::optional<NavigationDataFlightPlanAirport>& getAirport() const;

			void setAirport(const std::optional<NavigationDataFlightPlanAirport>& airport);

		protected:
			void onClick() override;

		private:
			std::optional<NavigationDataFlightPlanAirport> _airport = std::nullopt;
			bool _destination;
	};

	class LegFlightPlanItem : public FlightPlanItem {
		public:
			explicit LegFlightPlanItem(uint16_t legIndex);

		protected:
			void onClick() override;

		private:
			uint16_t _legIndex;
	};
}