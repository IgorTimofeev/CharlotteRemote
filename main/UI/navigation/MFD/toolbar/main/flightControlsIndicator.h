#pragma once

#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class FlightControlsIndicator : public Control {
		public:
			FlightControlsIndicator();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
		
	};
}