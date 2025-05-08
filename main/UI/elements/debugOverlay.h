#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class DebugOverlay : public Element {
		protected:
			void onRender(Renderer* renderer) override;

			void onTick() override;
	};
}