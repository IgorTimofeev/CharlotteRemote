#pragma once

#include <format>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>
#include <types/navigationData.h>
#include <utils/rendering.h>

namespace pizda {
	using namespace YOBA;

	class WaypointItem : public Button {
		public:
			explicit WaypointItem(uint16_t waypointIndex);

		protected:
			void onClick() override;
			void onRender(Renderer* renderer) override;

		private:
			uint16_t _waypointIndex;
			float _distanceNm;
	};
}