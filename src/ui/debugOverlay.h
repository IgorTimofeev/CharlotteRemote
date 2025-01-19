#pragma once

#include "../../lib/yoba/src/main.h"
#include "../../lib/yoba/src/ui.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class DebugOverlay : public Element {
		protected:
			void onRender(Renderer* renderer) override;
	};
}