#pragma once

#include "hardware/speaker/sound.h"

namespace pizda {
	class TransceiverDisconnectSound : public Sound {
		public:
			constexpr TransceiverDisconnectSound() : Sound(playables, playablesSize) {
			
			}
			
		private:
			constexpr static Note  p0 = { 900, 50'000 };
			constexpr static Delay p1 = { 50'000 };
			constexpr static Note  p2 = { 900, 50'000 };
			
			constexpr static uint8_t playablesSize = 3;
			
			constexpr static const Playable* playables[playablesSize] {
				&p0,
				&p1,
				&p2
			};
	};
}