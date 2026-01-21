#pragma once

#include <format>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/elements/navigation/waypointButton.h"

namespace pizda {
	using namespace YOBA;

	class FlightPlanItem : public WaypointButton {
		public:
			explicit FlightPlanItem(uint16_t legIndex);

		protected:
			void onClick() override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			uint16_t _legIndex;
	};
}