#pragma once

#include <hardware/speaker/sound.h>

namespace pizda::resources {
	class Sounds {
		public:
			static Sound boot() {
				return Sound({
					Note(7500, 50'000),
					Delay(50'000),
					Note(7500, 50'000)
				});
			}

			static Sound transceiverConnect() {
				return Sound({
					Note(5500, 50'000)
				});
			}

			static Sound transceiverDisconnect() {
				return Sound({
					Note(900, 50'000),
					Delay(50'000),
					Note(900, 50'000)
				});
			}
	};
}