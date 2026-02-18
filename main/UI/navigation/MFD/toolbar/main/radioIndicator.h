#pragma once

#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class RadioIndicator : public Control {
		public:
			RadioIndicator();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			constexpr static uint8_t _lineCount = 5;
			constexpr static uint8_t _lineSpacing = 2;
			constexpr static uint8_t _lineHeightMin = 2;
			constexpr static uint8_t _lineHeightIncrement = 3;
			
			constexpr static uint8_t _textOffset = 5;
			constexpr static uint8_t _textMaxWidth = 19;
	};
}