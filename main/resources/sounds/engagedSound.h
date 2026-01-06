#pragma once

#include <array>

#include "hardware/audio/sound.h"

namespace pizda {
	class EngagedSound : public Sound {
		public:
			constexpr EngagedSound() : Sound(playables.data(), playables.size()) {
			
			}
			
		private:
			constexpr static Note  p0 = { 5'000, 40'000 };
			constexpr static Delay p1 = { 40'000 };
			constexpr static Note  p2 = { 6'000, 40'000 };
			
			constexpr static std::array<const Playable*, 3> playables {
				&p0,
				&p1,
				&p2
			};
	};
}