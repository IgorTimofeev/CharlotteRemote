#pragma once

#include <cstdio>
#include "vector"

namespace pizda {
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

			explicit Sound(const std::vector<Note>& notes) : _notes(notes) {

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

			const std::function<void()>& getOnFinish() const {
				return _onFinish;
			}

			void setOnFinish(const std::function<void()>& value) {
				_onFinish = value;
			}

		private:
			std::vector<Note> _notes;
			size_t _noteIndex = 0;
			bool _repeating = false;
			std::function<void()> _onFinish;
	};
}