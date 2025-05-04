#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class DebugOverlay : public Element {
		protected:
			void onRender(Renderer* textSetter, const Bounds& bounds) override;

			void onTick() override;
	};
}