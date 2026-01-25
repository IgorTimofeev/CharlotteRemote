#pragma once

#include <stdint.h>

namespace pizda {
	#pragma pack(push, 1)
	class Playable {
		public:
			constexpr Playable(const bool delay, const uint32_t duration) : _delay(delay), _duration(duration) {
			
			}
			
			void setDelay(const bool delay) {
				_delay = delay;
			}
			
			bool isDelay() const {
				return _delay;
			}
			
			void setDuration(const uint32_t duration) {
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
			constexpr Delay(const uint32_t duration) : Playable(true, duration) {

			}
			
			constexpr Delay() : Delay(0) {
			
			}
	};
	
	class Note : public Playable {
		public:
			constexpr Note(const uint32_t frequency, const uint32_t duration) : Playable(false, duration), _frequency(frequency) {
			
			}
			
			constexpr Note() : Note(0, 0) {
			
			}
			
			uint32_t getFrequency() const {
				return _frequency;
			}
			
			void setFrequency(const uint32_t frequency) {
				_frequency = frequency;
			}
		
		private:
			uint32_t _frequency;
	};
	
	#pragma pack(pop)

	class Sound {
		public:
			constexpr Sound(const Playable* const* playables, const size_t playablesLength) : _playables(playables), _playablesLength(playablesLength) {
			
			}
			
			constexpr Sound() : Sound(nullptr, 0) {
			
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
			
			void setPlayablesLength(const size_t playablesLength) {
				_playablesLength = playablesLength;
			}
		
		private:
			const Playable* const* _playables;
			size_t _playablesLength;
	};
}