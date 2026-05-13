#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;
	
	class TrimControl : public SevenRotaryControl {
		public:
			TrimControl(std::wstring_view title, int8_t* settingsValue);
			
		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override;
			bool isVariantEditable(uint8_t index) override;
			void onRotate(bool clockwise, bool big) override;
			
		private:
			std::wstring _title;
			int8_t* _settingsValue;
	};
}