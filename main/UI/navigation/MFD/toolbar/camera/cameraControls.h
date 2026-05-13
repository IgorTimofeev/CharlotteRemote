#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/sevenRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	template<int8_t minimum, int8_t maximum>
	class CameraControl : public SevenRotaryControl<3, minimum, maximum, false, 1, 10> {
		public:
			CameraControl(const std::wstring_view title, int8_t* settingsValue) : _title(title), _settingsValue(settingsValue) {
				setVariants({
					&seven
				});

				setVariantIndex(0);

				seven.setSignVisible(true);
				seven.setValue(*_settingsValue);
			}

		protected:
			std::wstring_view variantIndexToTitle(uint8_t index) override {
				return _title;
			}

			bool isVariantEditable(uint8_t index) override {
				return true;
			}

			void onRotate(const bool clockwise, const bool big) override {
				SevenRotaryControl<3, minimum, maximum, false, 1, 10>::onRotate(clockwise, big);

				*_settingsValue = seven.getValue();
			}

		private:
			std::wstring _title;
			int8_t* _settingsValue;
	};
}