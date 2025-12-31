#pragma once

#include <array>

#include "hardware/audio/sound.h"

namespace pizda {
	class BootSound : public Sound {
		public:
			constexpr BootSound() : Sound(playables.data(), playables.size()) {
			
			}
			
		private:
			constexpr static Note  p0 = { 7500, 50'000 };
			constexpr static Delay p1 = { 50'000 };
			constexpr static Note  p2 = { 7500, 50'000 };
			
			constexpr static std::array<const Playable*, 3> playables {
				&p0,
				&p1,
				&p2
			};
	};
}