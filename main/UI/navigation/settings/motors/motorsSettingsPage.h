#pragma once

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
			
			void toSettings();
			
			Callback<> changed {};
			
		private:
			MotorConfiguration* _settings;
			
			Layout _mainLayout {};
			
			Rectangle _backgroundRect {};
			
			RelativeStackLayout _row {};
			
			TextField _min {};
			Titler _minTitle { L"Min", &_min };
			
			TextField _max {};
			Titler _maxTitle { L"Max", &_max };
			
			Button _reverse {};
			
			template<std::integral T>
			void addTextField(Titler& titler, TextField& textField, T value) {
				titler.title.setTextColor(&Theme::fg5);
				
				Theme::apply(&textField);
				textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
				textField.setText(std::to_wstring(value));

				textField.input += [this](const Key key, std::optional<std::wstring_view> text) {
					if (key == Key::enter)
						changed();
				};
				
				_row += &titler;
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