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
			~MotorsSettingsPage() override;

		private:
			static int32_t _scrollPosition;

			MotorEditor _throttleLeft { "Left", MotorType::throttleLeft };
			MotorEditor _throttleRight { "Right", MotorType::throttleRight };
			HorizontalSeparator _throttleSeparator {};

			TextView _aileronsTitle { "Ailerons" };
			MotorEditor _aileronLeft { "Left", MotorType::aileronLeft };
			MotorEditor _aileronRight { "Right", MotorType::aileronRight };
			HorizontalSeparator _aileronsSeparator {};

			TextView _flapsTitle { "Flaps" };
			MotorEditor _flapLeft { "Flap", MotorType::flapLeft };
			MotorEditor _flapRight { "Flap", MotorType::flapRight };
			HorizontalSeparator _flapsSeparator {};

			TextView _tailTitle { "Tail" };
			MotorEditor _tailLeft { "Left", MotorType::tailLeft };
			MotorEditor _tailRight { "Right", MotorType::tailRight };
			HorizontalSeparator _tailSeparator {};

			TextView _noseTitle { "Nose" };
			MotorEditor _cameraPitch { "Camera pitch", MotorType::cameraPitch };
			MotorEditor _cameraYaw { "Camera yaw", MotorType::cameraYaw };
			MotorEditor _noseWheel { "Wheel steering", MotorType::noseWheel };

			void penisula(TextView* text);
			void vaginoz(MotorEditor* motorEditor);
	};
}