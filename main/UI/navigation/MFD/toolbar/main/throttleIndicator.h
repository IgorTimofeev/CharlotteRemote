#pragma once

#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class ThrottleIndicator : public Control {
		public:
			ThrottleIndicator();
			
			void onRender(Renderer* pos, const Bounds& bounds) override;

			uint8_t getRemoteValue() const;
			void setRemoteValue(uint8_t value);

			uint8_t getAircraftValue() const;
			void setAircraftValue(uint8_t value);

		private:
			uint8_t _remoteValue = 0;
			uint8_t _aircraftValue = 0;
	};
}