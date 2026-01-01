#pragma once

#include <YOBA/UI.h>

#include "UI/theme.h"

namespace pizda {
	using namespace YOBA;

	class Separator : public Shape {
		public:
			Separator() {
				setHeight(1);
				setFillColor(&Theme::bg2);
			}
			
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto color = getFillColor();
				
				if (color)
					renderer->renderHorizontalLine(bounds.getTopLeft(), bounds.getWidth(), color);
			}
	};
}