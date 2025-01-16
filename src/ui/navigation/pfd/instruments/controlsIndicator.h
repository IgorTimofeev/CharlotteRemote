#pragma once

#include "ui/imageView.h"
#include "ui/theme.h"
#include "PFDFCSImage.h"

namespace pizdanc {
	using namespace yoba;

	class ControlsIndicator : public ImageView {
		public:
			ControlsIndicator();

			void render(Renderer* renderer) override;

		private:
			static const uint8_t maxAngle = 45;

			PFDFCSImage _PFDCSImage = PFDFCSImage();
	};
}