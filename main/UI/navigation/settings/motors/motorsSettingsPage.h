#pragma once

#include <functional>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/separator.h"

#include "types/generic.h"
#include "utilities/string.h"

namespace pizda {
	using namespace YOBA;
	
	class MotorEditor : public Titler {
		public:
			MotorEditor(std::wstring_view title, MotorType type);
			
			void toSettings() const;

		private:
			MotorType _type;

			Layout _mainLayout {};

			RelativeStackLayout _minMaxRow {};
			TextField _min {};
			TextField _max {};

			Button _reverse {};

			void changed() const;

			template<std::integral T>
			void addTextField(TextField& textField, T value) {
				Theme::apply(&textField);
				textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
				textField.setText(std::to_wstring(value));

				textField.setOnInput([this](const Key key, std::optional<std::wstring_view> text) {
					if (key == Key::enter)
						changed();
				});
				
				_minMaxRow += &textField;
			}
	};

	class MotorsSettingsPage : public ScrollViewPage {
		public:
			MotorsSettingsPage();
			
		private:
			MotorEditor throttle { L"Throttle", MotorType::throttle };
			MotorEditor noseWheel { L"Wheel steering", MotorType::noseWheel };
			HorizontalSeparator _mainSeparator {};

			TextView _leftWingTitle { L"Left wing" };
			MotorEditor flapLeft { L"Flap", MotorType::flapLeft };
			MotorEditor aileronLeft { L"Aileron", MotorType::aileronLeft };
			HorizontalSeparator _leftWingSeparator {};

			TextView _rightWingTitle { L"Right wing" };
			MotorEditor flapRight { L"Flap", MotorType::flapRight };
			MotorEditor aileronRight { L"Aileron", MotorType::aileronRight };
			HorizontalSeparator _rightWingSeparator {};

			TextView _tailTitle { L"Tail" };
			MotorEditor tailLeft { L"Left", MotorType::tailLeft };
			MotorEditor tailRight { L"Right", MotorType::tailRight };
			HorizontalSeparator _tailSeparator {};

			TextView _cameraTitle { L"Camera" };
			MotorEditor cameraPitch { L"Pitch", MotorType::cameraPitch };
			MotorEditor cameraYaw { L"Yaw", MotorType::cameraYaw };

			void penisula(TextView* text);
			void vaginoz(MotorEditor* motorEditor);
	};
}