#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "../route.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class OpenMenuButton : public Button {
		public:
			OpenMenuButton(const Route* route);
			OpenMenuButton();

		protected:
			void onClick() override;

		private:
			const Route* _route;
	};
}