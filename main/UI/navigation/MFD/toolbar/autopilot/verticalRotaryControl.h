#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class VerticalRotaryControlStab : public RotaryControlRenderable {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
	};

	class VerticalRotaryControlALT : public RotaryControlRenderable {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
	};

	class VerticalRotaryControl : public SevenRotaryControl<4, 0, 35000, false, 10, 100> {
		public:
			VerticalRotaryControl();

			VerticalRotaryControlStab stab {};
			VerticalRotaryControlALT ALT {};

		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onPress() override;
			void onRotate(bool clockwise, bool big) override;
			void onTick() override;
	};
}