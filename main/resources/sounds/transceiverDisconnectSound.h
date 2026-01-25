#pragma once

#include <array>

#include "systems/audio/sound.h"

namespace pizda {
	class TransceiverDisconnectSound : public Sound {
		public:
			constexpr TransceiverDisconnectSound() : Sound(playables.data(), playables.size()) {
			
			}
			
		private:
			constexpr static Note  p0 = { 900, 50'000 };
			constexpr static Delay p1 = { 50'000 };
			constexpr static Note  p2 = { 900, 50'000 };
			
			constexpr static std::array<const Playable*, 3> playables {
				&p0,
				&p1,
				&p2
			};
	};
}