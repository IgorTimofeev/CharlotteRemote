#pragma once

#include "hardware/axes/axis.h"

namespace pizda {
	class Axes {
		public:
			void setup();

			Axis& getLeverLeft() {
				return _leverLeft;
			}

			Axis& getLeverRight() {
				return _leverRight;
			}

			Axis& getJoystickHorizontal() {
				return _joystickHorizontal;
			}

			Axis& getJoystickVertical() {
				return _joystickVertical;
			}

			Axis& getRing() {
				return _ring;
			}

		private:
			Axis _leverLeft {};
			Axis _leverRight {};
			Axis _joystickHorizontal {};
			Axis _joystickVertical {};
			Axis _ring {};

			[[noreturn]] void onStart();
	};
}