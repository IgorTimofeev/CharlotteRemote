#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class OpenMenuButton : public Button {
		public:
			OpenMenuButton();

		protected:
			void onIsCheckedChanged() override;
	};
}