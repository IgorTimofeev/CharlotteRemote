#pragma once

#include <cstdint>
#include "number.h"

namespace yoba {
	class Color {
		public:
			virtual uint16_t to16Bit() const = 0;
	};

	class HsbColor {
		public:
			float _h = 0;
			float _s = 0;
			float _b = 0;

			HsbColor(float h, float s, float b) : _h(h), _s(s), _b(b) {

			}
	};

	class HighColor  : public Color {
		public:
			static const HighColor black;
			static const HighColor white;

			explicit HighColor(uint16_t value);

			uint16_t to16Bit() const override;

		private:
			uint16_t _value;
	};

	class TrueColor  : public Color {
		public:
			static const TrueColor black;
			static const TrueColor white;

			TrueColor(uint8_t r, uint8_t g, uint8_t b);

			TrueColor(TrueColor const &source);

			TrueColor(HsbColor const &source);

			TrueColor();

			explicit TrueColor(uint32_t value);

			void multiply(float factor);

			void add(uint8_t r, uint8_t g, uint8_t b);

			void add(const TrueColor& color);

			uint32_t to24Bit() const;

			uint16_t to16Bit() const override;

			static uint8_t interpolateChannel(uint8_t first, uint8_t second, float position);

			void interpolateTo(TrueColor& second, float position);

			uint8_t getR() const;
			void setR(uint8_t r);

			uint8_t getG() const;
			void setG(uint8_t g);

			uint8_t getB() const;
			void setB(uint8_t b);

		private:
			uint8_t _r = 0;
			uint8_t _g = 0;
			uint8_t _b = 0;
	};

	class PaletteColor : public Color {
		public:
			explicit PaletteColor(uint8_t index);

			uint16_t to16Bit() const override;

		private:
			uint8_t _index;
	};
}