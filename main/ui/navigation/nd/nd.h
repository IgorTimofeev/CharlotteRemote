#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"
#include "../../theme.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class ND : public Element {
		public:
			ND();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:

	};
}