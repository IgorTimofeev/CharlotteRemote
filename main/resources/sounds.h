#pragma once

#include "../sound.h"

namespace pizda::resources::sounds {
	static Sound boot() {
		return Sound({
			Note(7500, 80'000),
			Delay(80'000),
			Note(7500, 80'000)
		});
	}
}