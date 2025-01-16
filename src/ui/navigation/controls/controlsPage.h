#pragma once

#include "Arduino.h"

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "../page.h"
#include "../../resources/controlsImage.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class ControlsView : public ImageView {
		public:
			ControlsView();

			static const ControlsImage controlsImage;

			void onRender(Renderer* renderer) override;

		private:
			const uint8_t maxPixelValue = 12;
	};

	class ControlsPage : public RowsPage {
		public:
			ControlsPage() {
				rows.setAlignment(Alignment::Center);

				rows += &controls;
			}

			ControlsView controls = ControlsView();
	};
}