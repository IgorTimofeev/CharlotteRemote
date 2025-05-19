#pragma once

#include <format>

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class WaypointButton : public Button {
		public:
			explicit WaypointButton();
			explicit WaypointButton(uint32_t waypointIndex);

			int32_t getWaypointIndex() const;
			void setWaypointIndex(int32_t value);

		protected:
			void onRender(Renderer* renderer) override;

		private:
			float _distanceNm;
			int32_t _waypointIndex = -1;
	};
}