#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/imageView.h"

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/titler.h"

#include "backImage.h"

using namespace yoba;

namespace pizdanc {
	class PizdaImageView : public ImageView {
		public:
			PizdaImageView();

			static const BackImage backImage;

			void onRender(ScreenBuffer* screenBuffer) override;

		private:
			const uint8_t maxPixelValue = 12;
	};

	class ControlsPage : public RowsPage {
		public:
			ControlsPage() {
				rows.setAlignment(Alignment::Center);

				rows += &imageView;
			}

			PizdaImageView imageView = PizdaImageView();
	};
}