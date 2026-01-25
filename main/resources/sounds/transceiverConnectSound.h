#pragma once

#include <array>

#include "systems/audio/sound.h"

namespace pizda {
	class TransceiverConnectSound : public Sound {
		public:
			constexpr TransceiverConnectSound() : Sound(playables.data(), playables.size()) {
			
			}
			
		private:
			constexpr static Note  p0 = { 5500, 50'000 };
			
			constexpr static std::array<const Playable*, 1> playables {
				&p0
			};
	};
}