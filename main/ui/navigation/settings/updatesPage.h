#pragma once

#include "../../components/yoba/src/main.h"
#include "../../components/yoba/src/ui.h"

#include "../page.h"
#include "../../theme.h"
#include "../../titler.h"
#include "../../switcher.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class UpdatesPage : public ScrollViewPage {
		public:
			UpdatesPage();

			Text _description;
			Button _button;
	};
}