#pragma once

#include <cstdio>
#include "vector"

namespace pizda {
	#pragma pack(push, 1)
	class Playable {
		public:
			constexpr Playable(bool delay, uint32_t duration) : _delay(delay), _duration(duration) {
			
			}
			
			void setDelay(bool delay) {
				_delay = delay;
			}
			
			bool isDelay() const {
				return _delay;
			}
			
			void setDuration(uint32_t duration) {
				_duration = duration;
			}
			
			uint32_t getDuration() const {
				return _duration;
			}
			
		private:
			bool _delay: 1;
			uint32_t _duration;
	};

	class Delay : public Playable {
		public:
			constexpr Delay(uint32_t duration) : Playable(true, duration) {

			}
			
			constexpr Delay() : Delay(0) {
			
			}
	};
	
	class Note : public Playable {
		public:
			constexpr Note(uint32_t frequency, uint32_t duration) : Playable(false, duration), _frequency(frequency) {
			
			}
			
			constexpr Note() : Note(0, 0) {
			
			}
			
			uint32_t getFrequency() const {
				return _frequency;
			}
			
			void setFrequency(uint32_t frequency) {
				_frequency = frequency;
			}
		
		private:
			uint32_t _frequency;
	};
	
	#pragma pack(pop)

	class Sound {
		public:
			constexpr Sound() : _playables(nullptr), _playablesLength(0) {
			
			}
			
			constexpr Sound(const Playable* const* playables, size_t playablesLength) : _playables(playables), _playablesLength(playablesLength) {
			
			}
			
			const Playable* const* getPlayables() const {
				return _playables;
			}
			
			void setPlayables(const Playable** playables) {
				_playables = playables;
			}
			
			size_t getPlayablesLength() const {
				return _playablesLength;
			}
			
			void setPlayablesLength(size_t playablesLength) {
				_playablesLength = playablesLength;
			}
		
		private:
			const Playable* const* _playables;
			size_t _playablesLength;
	};
}