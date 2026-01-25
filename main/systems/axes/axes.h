#pragma once

#include "systems/axes/axis.h"

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
			int64_t _tickTime = 0;

			Axis _leverLeft {};
			Axis _leverRight {};
			Axis _joystickHorizontal {};
			Axis _joystickVertical {};
			Axis _ring {};
	};
}