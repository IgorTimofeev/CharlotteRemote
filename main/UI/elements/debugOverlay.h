#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class DebugOverlay : public Control {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onTick() override;
	};
}