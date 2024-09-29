#pragma once

#include "ui/imageView.h"
#include "ui/theme.h"
#include "PFDFCSImage.h"

using namespace yoba;

namespace pizdanc {
	class ControlsIndicator : public ImageView {
		public:
			ControlsIndicator();

			void render(ScreenBuffer* screenBuffer) override;

		private:
			static const uint8_t maxAngle = 45;

			PFDFCSImage _PFDCSImage = PFDFCSImage();
	};
}