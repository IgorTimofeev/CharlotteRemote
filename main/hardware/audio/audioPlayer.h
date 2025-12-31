#pragma once

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "hardware/audio/speaker.h"
#include "hardware/audio/sound.h"

namespace pizda {
	class AudioPlayer {
		public:
			void setup();
			void play(const Sound& sound);
			void playFeedback();

		private:
			Speaker _speaker {};
			
			TaskHandle_t _taskHandle = nullptr;
			const Sound* _sound = nullptr;
			size_t _playableIndex = 0;
			uint32_t _playableDeadline = 0;
			
			[[noreturn]] void onStart();
	};
}