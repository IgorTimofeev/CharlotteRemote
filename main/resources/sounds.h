#pragma once

#include "../sound.h"

namespace pizda::resources::sounds {
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
}