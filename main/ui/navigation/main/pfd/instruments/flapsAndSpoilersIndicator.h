#pragma once

#include "../../../../../resources/images/PFDFlapsAndSpoilersImage.h"
#include "src/ui.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class FlapsAndSpoilersIndicator : public ImageView {
		public:
			FlapsAndSpoilersIndicator();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			constexpr static const uint8_t maxAngle = 45;

			PFDFlapsAndSpoilersImage _image {};
	};
}