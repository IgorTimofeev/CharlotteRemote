#pragma once

#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class FlapsAndSpoilersIndicator : public ImageView {
		public:
			FlapsAndSpoilersIndicator();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			constexpr static uint8_t maxAngle = 45;
	};
}