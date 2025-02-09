#pragma once

#include "../sound.h"

namespace pizdanc::resources::sounds {
	static const Sound& boot() {
		return Sound({
			Note(7500, 40000),
			Delay(40000),
			Note(7500, 40000)
		});
	}
}