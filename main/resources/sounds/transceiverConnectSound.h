#pragma once

#include "hardware/speaker/sound.h"

namespace pizda {
	class TransceiverConnectSound : public Sound {
		public:
			constexpr TransceiverConnectSound() : Sound(playables, playablesSize) {
			
			}
			
		private:
			constexpr static Note  p0 = { 5500, 50'000 };
			
			constexpr static uint8_t playablesSize = 1;
			
			constexpr static const Playable* playables[playablesSize] {
				&p0
			};
	};
}