#pragma once

#include "PFDFCSImage.h"
#include "../../../components/yoba/src/ui.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ControlsIndicator : public ImageView {
		public:
			ControlsIndicator();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			constexpr static const uint8_t maxAngle = 45;

			PFDFCSImage _image {};
	};
}