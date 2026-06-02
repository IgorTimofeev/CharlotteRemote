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
			MotorEditor(std::string_view title, MotorType type);
			
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
				textField.setText(std::to_string(value));

				textField.setOnInput([this](const Key key, std::optional<std::string_view> text) {
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
			MotorEditor _throttle { "Throttle", MotorType::throttle };
			// MotorEditor _reverse { "Reverse", MotorType::reverse };
			MotorEditor _noseWheel { "Wheel", MotorType::noseWheel };
			HorizontalSeparator _mainSeparator {};

			TextView _leftWingTitle { "Left wing" };
			MotorEditor _flapLeft { "Flap", MotorType::flapLeft };
			MotorEditor _aileronLeft { "Aileron", MotorType::aileronLeft };
			HorizontalSeparator _leftWingSeparator {};

			TextView _rightWingTitle { "Right wing" };
			MotorEditor _flapRight { "Flap", MotorType::flapRight };
			MotorEditor _aileronRight { "Aileron", MotorType::aileronRight };
			HorizontalSeparator _rightWingSeparator {};

			TextView _tailTitle { "Tail" };
			MotorEditor _tailLeft { "Left", MotorType::tailLeft };
			MotorEditor _tailRight { "Right", MotorType::tailRight };
			HorizontalSeparator _tailSeparator {};

			TextView _cameraTitle { "Camera" };
			MotorEditor _cameraPitch { "Pitch", MotorType::cameraPitch };
			MotorEditor _cameraYaw { "Yaw", MotorType::cameraYaw };

			void penisula(TextView* text);
			void vaginoz(MotorEditor* motorEditor);
	};
}