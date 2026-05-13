#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class VerticalRotaryControlStab : public RotaryControlSevenVariant {
		public:
			VerticalRotaryControlStab();

		protected:
			void onTick() override;
	};

	class VerticalRotaryControlALT : public RotaryControlSevenVariant {
		public:
			VerticalRotaryControlALT();

		protected:
			void onTick() override;
	};

	class VerticalRotaryControl : public SevenRotaryControl {
		public:
			VerticalRotaryControl();

			VerticalRotaryControlStab stab {};
			VerticalRotaryControlALT ALT {};

		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onPress() override;
			void onRotate(bool clockwise, bool big) override;
			void onVariantChanged() override;
			void onTick() override;
	};
}