#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class CameraControl : public SevenRotaryControl {
		public:
			CameraControl(
				const int8_t minimum,
				const int8_t maximum,
				const std::wstring_view title,
				int16_t* settingsValue
			);

		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;

			bool isVariantEditable(uint8_t index) override;

			void onRotate(const bool clockwise, const bool big) override;

		private:
			std::wstring _title;
			int16_t* _settingsValue;
	};
}