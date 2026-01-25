#pragma once

#include <array>

#include "systems/audio/sound.h"

namespace pizda {
	class FeedbackSound : public Sound {
		public:
			constexpr FeedbackSound() : Sound(playables.data(), playables.size()) {
			
			}
			
		private:
			constexpr static Note p0 = { 12'000, 20'000 };
			
			constexpr static std::array<const Playable*, 1> playables {
				&p0
			};
	};
}