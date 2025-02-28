#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../resources/images/controlsImage.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ControlsView : public ImageView {
		public:
			ControlsView();

			static const ControlsImage controlsImage;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			const uint8_t maxPixelValue = 12;
	};
}