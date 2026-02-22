#pragma once

#include <format>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/theme.h"

namespace pizda {
	using namespace YOBA;

	class WaypointButton : public Button {
		public:
			explicit WaypointButton();
			explicit WaypointButton(uint32_t waypointIndex);

			constexpr static uint8_t height = Theme::elementHeight;

			int32_t getWaypointIndex() const;
			void setWaypointIndex(int32_t value);

			float getDistanceNm() const;

		protected:
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			float _distanceNm;
			int32_t _waypointIndex = -1;
	};
}