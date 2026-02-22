#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class PFDPageSplitter : public Control, public ActiveElement {
		protected:
			void onEvent(Event& event) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			int32_t _pointerY = -1;
			bool _draggedDohuya = false;
	};
}