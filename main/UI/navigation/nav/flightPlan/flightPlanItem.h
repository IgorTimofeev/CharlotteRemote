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
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			virtual std::wstring getNameForRendering(const NavigationWaypointData& waypointData) const;
	};

	class AirportFlightPlanItem : public FlightPlanItem {
		public:
			explicit AirportFlightPlanItem(bool destination);

			const std::optional<NavigationAirportAndRunwayIndicesData>& getAirportAndRunway() const;
			void setAirportAndRunway(const std::optional<NavigationAirportAndRunwayIndicesData>& airportAndRunway);

		protected:
			void onClick() override;

			std::wstring getNameForRendering(const NavigationWaypointData& waypointData) const override;

		private:
			std::optional<NavigationAirportAndRunwayIndicesData> _airportAndRunway = std::nullopt;
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