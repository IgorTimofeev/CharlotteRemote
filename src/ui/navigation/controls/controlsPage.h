#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/imageView.h"

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/titler.h"

#include "backImage.h"

using namespace yoba;

namespace pizdanc {
	class ControlsPage : public RowsPage {
		public:
			ControlsPage() {
				rows.setAlignment(Alignment::Center);

				rows += &imageView;
			}

			static const BackImage backImage;

			ImageView imageView = ImageView(&backImage);
	};
}