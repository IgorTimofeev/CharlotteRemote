#pragma once

#include "../../../../lib/YOBA/src/hardware/screen/buffers/screenBuffer.h"
#include "../../../../lib/YOBA/src/ui/element.h"
#include "../../../../lib/YOBA/src/ui/shapes/rectangle.h"
#include "../../../../lib/YOBA/src/ui/stackLayout.h"
#include "../../../elements/titler.h"
#include "trimIndicator.h"

using namespace yoba;

namespace pizdanc {
	class InstrumentsLayout : public Layout {
		public:
			InstrumentsLayout();

			void tick();

		private:
			StackLayout _rows = StackLayout();

			Rectangle _backgroundRect = Rectangle(&Theme::bg2);
			TrimIndicator _pitchTrimIndicator = TrimIndicator();
			Titler _pitchTitle = Titler(L"Trim", &_pitchTrimIndicator);
	};
}