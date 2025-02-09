#pragma once

#include <cstdio>
#include "driver/ledc.h"
#include "esp_err.h"
#include "queue"
#include "forward_list"
#include "../constants.h"

namespace pizdanc {
	class Note {
		public:
			Note(uint32_t frequency, uint32_t duration) : _frequency(frequency), _duration(duration) {

			}

			uint32_t getFrequency() const {
				return _frequency;
			}

			uint32_t getDuration() const {
				return _duration;
			}

		private:
			uint32_t _frequency;
			uint32_t _duration;
	};

	class Delay : public Note {
		public:
			explicit Delay(uint32_t duration) : Note(0, duration) {

			}
	};

	class Sound {
		public:
			Sound() : _notes({}) {

			}

			Sound(const std::initializer_list<Note>& notes) : _notes(notes) {

			}

			static Sound threeBeeps() {
				return {
					Note(4000, 100000),
					Note(4000, 100000),
					Note(4000, 100000),
				};
			}

			std::vector<Note>& getNotes() {
				return _notes;
			}

			void setNotes(const std::vector<Note>& value) {
				_notes = value;
			}

			size_t getNoteIndex() const {
				return _noteIndex;
			}

			void setNoteIndex(size_t noteIndex) {
				_noteIndex = noteIndex;
			}

			bool isRepeating() const {
				return _repeating;
			}

			void setRepeating(bool value) {
				_repeating = value;
			}

			const std::optional<std::function<void()>>& getOnFinish() const {
				return _onFinish;
			}

			void setOnFinish(const std::optional<std::function<void()>>& value) {
				_onFinish = value;
			}

		private:
			std::vector<Note> _notes;
			size_t _noteIndex = 0;
			bool _repeating = false;
			std::optional<std::function<void()>> _onFinish = std::nullopt;
	};

	class Speaker {
		public:
			void setup() const {
				ledc_timer_config_t timerConfig {};
				timerConfig.speed_mode = constants::hardware::speaker::mode;
				timerConfig.duty_resolution = LEDC_TIMER_13_BIT;
				timerConfig.timer_num = constants::hardware::speaker::timer;
				timerConfig.freq_hz = 4000;
				timerConfig.clk_cfg = LEDC_AUTO_CLK;
				ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

				ledc_channel_config_t channelConfig {};
				channelConfig.gpio_num = constants::hardware::speaker::gpio;
				channelConfig.speed_mode = constants::hardware::speaker::mode;
				channelConfig.channel = constants::hardware::speaker::channel;
				channelConfig.intr_type = LEDC_INTR_DISABLE;
				channelConfig.timer_sel = constants::hardware::speaker::timer;
				channelConfig.duty = 0;
				channelConfig.hpoint = 0;
				ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));
			}

			void play(const Sound& sound) {
				_tracks.push_back(Track {
					.sound = sound
				});
			}

			void tick() {
				// Playing only one sound per time, but still process other sounds timings
				for (size_t i = 0; i < _tracks.size(); i++) {
					auto& track = _tracks[i];

					// Skipping track if we're already playing its note
					if (esp_timer_get_time() < track.noteDeadline)
						continue;

					auto& note = track.sound.getNotes()[track.sound.getNoteIndex()];

					// First play
					if (track.noteDeadline == 0) {
						ESP_LOGI("Speaker", "First play");

						playNote(track, note);
					}
					// End of note
					else {
						ESP_LOGI("Speaker", "End of note");

						// Moving to next note
						track.sound.setNoteIndex(track.sound.getNoteIndex() + 1);

						// End of sound
						if (track.sound.getNoteIndex() >= track.sound.getNotes().size()) {
							ESP_LOGI("Speaker", "End of track");

							// Calling onFinish callback
							if (track.sound.getOnFinish().has_value())
								track.sound.getOnFinish().value()();

							// If track repeating enabled
							if (track.sound.isRepeating()) {
								track.sound.setNoteIndex(0);
								playNote(track);
							}
							// Erasing track otherwise
							else {
								auto it = _tracks.begin();
								std::advance(it, i);
								_tracks.erase(it);

								i--;

								// Turning off speaker if there are no tracks remaining
								if (_tracks.empty())
									setDuty(false);
							}
						}
						// Playing note
						else {
							ESP_LOGI("Speaker", "Next note");

							playNote(track);
						}
					}
				}
			}

		private:
			struct Track {
				Sound sound;
				uint32_t noteDeadline = 0;
			};

			std::vector<Track> _tracks {};

			static void setDuty(bool value) {
				ESP_ERROR_CHECK(ledc_set_duty(constants::hardware::speaker::mode, constants::hardware::speaker::channel, value ? 4096 : 0));
				ESP_ERROR_CHECK(ledc_update_duty(constants::hardware::speaker::mode, constants::hardware::speaker::channel));
			}

			void setFrequency(uint32_t value) {
				ESP_ERROR_CHECK(ledc_set_freq(constants::hardware::speaker::mode, constants::hardware::speaker::timer, value));
			}

			void playNote(Track& track, const Note& note) {
				// Regular note
				if (note.getFrequency() > 0) {
					setFrequency(note.getFrequency());
					setDuty(true);
				}
				// Delay
				else {
					setDuty(false);
				}

				track.noteDeadline = esp_timer_get_time() + note.getDuration();
			}

			void playNote(Track& track) {
				playNote(track, track.sound.getNotes()[track.sound.getNoteIndex()]);
			}
	};
}