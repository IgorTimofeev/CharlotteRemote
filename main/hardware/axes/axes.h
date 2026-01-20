#pragma once

#include "hardware/axes/axis.h"

namespace pizda {
	class Axes {
		public:
			void setup();

			Axis& getLeverLeft();
			Axis& getLeverRight();
			Axis& getJoystickHorizontal();
			Axis& getJoystickVertical();
			Axis& getRing();

			void tick();

		private:
			Axis _leverLeft {};
			Axis _leverRight {};
			Axis _joystickHorizontal {};
			Axis _joystickVertical {};
			Axis _ring {};

			int64_t _tickTime = 0;
	};
}