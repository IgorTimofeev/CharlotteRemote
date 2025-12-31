#pragma once

#include "hardware/speaker/sound.h"

namespace pizda {
class BootSound : public Sound {
	public:
		constexpr BootSound() : Sound(playables, playablesSize) {
		
		}
		
	private:
		constexpr static Note  p0 = { 7500, 50'000 };
		constexpr static Delay p1 = { 50'000 };
		constexpr static Note  p2 = { 7500, 50'000 };
		
		constexpr static uint8_t playablesSize = 3;
		
		constexpr static const Playable* playables[playablesSize] {
			&p0,
			&p1,
			&p2
		};
};
}