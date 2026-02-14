#pragma once

#include <functional>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"

#include "types/generic.h"
#include "utilities/string.h"

namespace pizda {
	using namespace YOBA;
	
	class MotorEditor : public Titler {
		public:
			MotorEditor(std::wstring_view title, MotorConfiguration* settings);
			
			void toSettings() const;
			
			std::function<void()> changed = nullptr;
			
		private:
			MotorConfiguration* _settings;
			
			Layout _mainLayout {};

			Rectangle _backgroundRect {};
			
			RelativeStackLayout _minMaxRow {};
			TextField _min {};
			TextField _max {};

			Button _reverse {};
			
			template<std::integral T>
			void addTextField(TextField& textField, T value) {
				Theme::apply(&textField);
				textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
				textField.setText(std::to_wstring(value));

				textField.setOnInput([this](const Key key, std::optional<std::wstring_view> text) {
					if (key == Key::enter)
						if (changed)
							changed();
				});
				
				_minMaxRow += &textField;
			}
	};

	class MotorsSettingsPage : public ScrollViewPage {
		public:
			MotorsSettingsPage();
			
		private:
			MotorEditor throttle;
			MotorEditor noseWheel;
			
			MotorEditor flapLeft;
			MotorEditor aileronLeft;

			MotorEditor flapRight;
			MotorEditor aileronRight;

			MotorEditor tailLeft;
			MotorEditor tailRight;
			
			void vaginoz(MotorEditor* motorEditor);
	};
}