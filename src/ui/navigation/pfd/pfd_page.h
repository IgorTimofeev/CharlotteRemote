#pragma once

#include "Arduino.h"

#include "yoba/elements/slider.h"
#include "yoba/elements/text.h"

#include "ui/navigation/page.h"
#include "ui/elements/titler.h"
#include "ui/elements/rows.h"
#include "pfd.h"

using namespace yoba;

namespace ui {
	class PFDPage : public Page {
		public:
			PFDPage() {
				// Horizon
				const auto& horizonMargin = Margin(34, 0, 34, 0);
				_horizon.setMargin(horizonMargin);
				addChild(&_horizon);

				// Left
				_left.setHorizontalAlignment(Alignment::start);
				_left.setSize(Size(horizonMargin.getLeft(), Size::calculated));
				addChild(&_left);

				// Right
				_right.setHorizontalAlignment(Alignment::end);
				_right.setSize(Size(horizonMargin.getRight(), Size::calculated));
				addChild(&_right);
			}

		private:
			PFDLeft _left = PFDLeft();
			PFDHorizon _horizon = PFDHorizon();
			PFDRight _right = PFDRight();
	};
}