#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../route.h"

namespace pizda {
	using namespace YOBA;

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