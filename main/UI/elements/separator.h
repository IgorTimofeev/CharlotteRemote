#pragma once

#include <YOBA/UI.h>

#include "UI/theme.h"

namespace pizda {
	using namespace YOBA;

	class HorizontalSeparator : public Shape {
		public:
			HorizontalSeparator() {
				setHeight(1);
				setFillColor(&Theme::bg3);
			}
			
			void onRender(Renderer& renderer, const Bounds& bounds) override {
				auto color = getFillColor();
				
				if (color)
					renderer.renderHorizontalLine(bounds.getTopLeft(), bounds.getWidth(), color);
			}
	};
	
	class VerticalSeparator : public Shape {
		public:
			VerticalSeparator() {
				setWidth(1);
				setFillColor(&Theme::bg3);
			}
			
			void onRender(Renderer& renderer, const Bounds& bounds) override {
				auto color = getFillColor();
				
				if (color)
					renderer.renderVerticalLine(bounds.getTopLeft(), bounds.getHeight(), color);
			}
	};
}