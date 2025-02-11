#pragma once

#include "../../components/yoba/src/main.h"
#include "../../components/yoba/src/ui.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class DebugOverlay : public Element {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
	};
}