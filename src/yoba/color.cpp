#include "color.h"
#include "number.h"

namespace yoba {
	const TrueColor TrueColor::black = TrueColor(0x00, 0x00, 0x00);
	const TrueColor TrueColor::white = TrueColor(0xFF, 0xFF, 0xFF);

	const HighColor HighColor::black = HighColor(0x0000);
	const HighColor HighColor::white = HighColor(0xFFFF);

	TrueColor::TrueColor(uint8_t r, uint8_t g, uint8_t b) :
		_r(r),
		_g(g),
		_b(b)
	{

	}

	TrueColor::TrueColor(const TrueColor &source) {
		_r = source._r;
		_g = source._g;
		_b = source._b;
	}

	TrueColor::TrueColor(const HsbColor &hsb) {
		auto hueSector = hsb._h * 6.0f;
		auto hueSectorIntegerPart = (uint8_t) hueSector;
		auto hueSectorFractionalPart = hueSector - (float) hueSectorIntegerPart;

		auto
			p = (uint8_t) (255.0f * hsb._b * (1 - hsb._s)),
			q = (uint8_t) (255.0f * hsb._b * (1 - hueSectorFractionalPart * hsb._s)),
			t = (uint8_t) (255.0f * hsb._b * (1 - (1 - hueSectorFractionalPart) * hsb._s)),
			v = (uint8_t) (255.0f * hsb._b);

		switch (hueSectorIntegerPart) {
			case 1:
				_r = q;
				_g = v;
				_b = p;
				break;

			case 2:
				_r = p;
				_g = v;
				_b = t;
				break;

			case 3:
				_r = p;
				_g = q;
				_b = v;
				break;

			case 4:
				_r = t;
				_g = p;
				_b = v;
				break;

			case 5:
				_r = v;
				_g = p;
				_b = q;
				break;

			default:
				_r = v;
				_g = t;
				_b = p;
				break;
		}
	}

	TrueColor::TrueColor() {
		_r = 0;
		_g = 0;
		_b = 0;
	}

	TrueColor::TrueColor(uint32_t value) {
		_r = value >> 16 & 0xFF;
		_g = value >> 8 & 0xFF;
		_b = value & 0xFF;
	}

	void TrueColor::add(const TrueColor &color) {
		add(color._r, color._g, color._b);
	}

	void TrueColor::add(uint8_t r, uint8_t g, uint8_t b) {
		_r = (uint8_t) Number::clampInt32(_r + r, 0, 255);
		_g = (uint8_t) Number::clampInt32(_g + g, 0, 255);
		_b = (uint8_t) Number::clampInt32(_b + b, 0, 255);
	}

	void TrueColor::multiply(float factor) {
		_r = (uint8_t) Number::clampFloat((float) _r * factor, 0.0f, 255.0f);
		_g = (uint8_t) Number::clampFloat((float) _g * factor, 0.0f, 255.0f);
		_b = (uint8_t) Number::clampFloat((float) _b * factor, 0.0f, 255.0f);
	}

	uint32_t TrueColor::to24Bit() const {
		return _r << 16 | _g << 8 | _b;
	}

	uint16_t TrueColor::to16Bit() const {
		uint32_t color888 = to24Bit();

		uint16_t r = (color888 >> 8) & 0xF800;
		uint16_t g = (color888 >> 5) & 0x07E0;
		uint16_t b = (color888 >> 3) & 0x001F;

		return (r | g | b);
	}

	uint8_t TrueColor::interpolateChannel(uint8_t first, uint8_t second, float position) {
		return (uint8_t) ((float) first + (float) (second - first) * position);
	}

	void TrueColor::interpolateTo(TrueColor &second, float position) {
		_r = interpolateChannel(_r, second._r, position);
		_g = interpolateChannel(_g, second._g, position);
		_b = interpolateChannel(_b, second._b, position);
	}

	uint8_t TrueColor::getR() const {
		return _r;
	}

	void TrueColor::setR(uint8_t r) {
		_r = r;
	}

	uint8_t TrueColor::getG() const {
		return _g;
	}

	void TrueColor::setG(uint8_t g) {
		_g = g;
	}

	uint8_t TrueColor::getB() const {
		return _b;
	}

	void TrueColor::setB(uint8_t b) {
		_b = b;
	}

	// ------------------------------------------------------------------------------------

	HighColor::HighColor(uint16_t value) : _value(value) {

	}

	uint16_t HighColor::to16Bit() const {
		return _value;
	}

	// ------------------------------------------------------------------------------------

	PaletteColor::PaletteColor(uint8_t index) : _index(index) {

	}

	uint16_t PaletteColor::to16Bit() const {
		return _index;
	}
}