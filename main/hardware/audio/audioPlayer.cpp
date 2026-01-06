#include "audioPlayer.h"

#include <esp_err.h>
#include <esp_timer.h>
#include <esp_log.h>

#include "rc.h"
#include "resources/sounds.h"

namespace pizda {
	void AudioPlayer::setup() {
		_speaker.setup();
		
		xTaskCreate(
			[](void* arg) {
				reinterpret_cast<AudioPlayer*>(arg)->onStart();
			},
			"AudioPlayer",
			4 * 1024,
			this,
			5,
			&_taskHandle
		);
	}
	
	void AudioPlayer::onStart() {
		while (true) {
			// Waiting for sound
			if (!_sound)
				ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			
			if (!_sound) {
				ESP_LOGE("AudioPlayer", "sound is null");
				continue;
			}
			else if (esp_timer_get_time() < _playableDeadline) {
				taskYIELD();
				continue;
			}
			
			if (_playableIndex < _sound->getPlayablesLength()) {
				const auto playable = _sound->getPlayables()[_playableIndex];
				
				if (playable->isDelay()) {
					_speaker.setState(false);
				}
				else {
					const auto note = reinterpret_cast<const Note*>(playable);
					
					_speaker.setFrequency(note->getFrequency());
					_speaker.setState(true);
				}
				
				_playableIndex = _playableIndex + 1;
				_playableDeadline = esp_timer_get_time() + playable->getDuration();
			}
			else {
				// Turning off speaker
				_sound = nullptr;
				_playableIndex = 0;
				_playableDeadline = 0;
				
				_speaker.setState(false);
			}
			
			taskYIELD();
		}
	}
	
	void AudioPlayer::play(const Sound& sound) {
		_sound = &sound;
		_playableIndex = 0;
		_playableDeadline = 0;
		
		xTaskNotifyGive(_taskHandle);
	}
	
	void AudioPlayer::playFeedback(const Sound& sound) {
		auto& rc = RC::getInstance();
		
		if (rc.getSettings().personalization.audioFeedback) {
			play(sound);
		}
	}
	
	void AudioPlayer::playFeedback() {
		playFeedback(resources::sounds::feedback);
	}
}