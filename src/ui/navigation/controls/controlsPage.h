#pragma once

#include "Arduino.h"

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "../page.h"
#include "../../elements/titler.h"

#include "backImage.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class PizdaImageView : public ImageView {
		public:
			PizdaImageView();

			static const BackImage backImage;

			void onRender(Renderer* renderer) override;

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