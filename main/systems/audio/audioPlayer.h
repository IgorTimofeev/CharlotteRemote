#pragma once

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "systems/audio/speaker.h"
#include "systems/audio/sound.h"

namespace pizda {
	class AudioPlayer {
		public:
			void setup();
			void play(const Sound* sound);
			void playFeedback(const Sound* sound);
			void playFeedback();

		private:
			Speaker _speaker {};
			
			TaskHandle_t _taskHandle = nullptr;
			const Sound* _sound = nullptr;
			size_t _playableIndex = 0;
			int64_t _playableDeadline = 0;
			
			[[noreturn]] void onStart();
	};
}