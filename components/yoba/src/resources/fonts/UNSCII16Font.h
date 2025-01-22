﻿#pragma once

#include "main/font.h"

namespace yoba {
	class UNSCII16Font : public Font {
		public:
			UNSCII16Font() : Font(
				32,
				126,
				18,
				_glyphs,
				_bitmap
			) {

			}

		private:
			const Glyph _glyphs[95] = {
				Glyph(0, 8), //
				Glyph(144, 8), // !
				Glyph(288, 8), // "
				Glyph(432, 8), // #
				Glyph(576, 8), // $
				Glyph(720, 8), // %
				Glyph(864, 8), // &
				Glyph(1008, 8), // '
				Glyph(1152, 8), // (
				Glyph(1296, 8), // )
				Glyph(1440, 8), // *
				Glyph(1584, 8), // +
				Glyph(1728, 8), // ,
				Glyph(1872, 8), // -
				Glyph(2016, 8), // .
				Glyph(2160, 8), // /
				Glyph(2304, 8), // 0
				Glyph(2448, 8), // 1
				Glyph(2592, 8), // 2
				Glyph(2736, 8), // 3
				Glyph(2880, 8), // 4
				Glyph(3024, 8), // 5
				Glyph(3168, 8), // 6
				Glyph(3312, 8), // 7
				Glyph(3456, 8), // 8
				Glyph(3600, 8), // 9
				Glyph(3744, 8), // :
				Glyph(3888, 8), // ;
				Glyph(4032, 8), // <
				Glyph(4176, 8), // =
				Glyph(4320, 8), // >
				Glyph(4464, 8), // ?
				Glyph(4608, 8), // @
				Glyph(4752, 8), // A
				Glyph(4896, 8), // B
				Glyph(5040, 8), // C
				Glyph(5184, 8), // D
				Glyph(5328, 8), // E
				Glyph(5472, 8), // F
				Glyph(5616, 8), // G
				Glyph(5760, 8), // H
				Glyph(5904, 8), // I
				Glyph(6048, 8), // J
				Glyph(6192, 8), // K
				Glyph(6336, 8), // L
				Glyph(6480, 8), // M
				Glyph(6624, 8), // N
				Glyph(6768, 8), // O
				Glyph(6912, 8), // P
				Glyph(7056, 8), // Q
				Glyph(7200, 8), // R
				Glyph(7344, 8), // S
				Glyph(7488, 8), // T
				Glyph(7632, 8), // U
				Glyph(7776, 8), // V
				Glyph(7920, 8), // W
				Glyph(8064, 8), // X
				Glyph(8208, 8), // Y
				Glyph(8352, 8), // Z
				Glyph(8496, 8), // [
				Glyph(8640, 8), // backslash
				Glyph(8784, 8), // ]
				Glyph(8928, 8), // ^
				Glyph(9072, 8), // _
				Glyph(9216, 8), // `
				Glyph(9360, 8), // a
				Glyph(9504, 8), // b
				Glyph(9648, 8), // c
				Glyph(9792, 8), // d
				Glyph(9936, 8), // e
				Glyph(10080, 8), // f
				Glyph(10224, 8), // g
				Glyph(10368, 8), // h
				Glyph(10512, 8), // i
				Glyph(10656, 8), // j
				Glyph(10800, 8), // k
				Glyph(10944, 8), // l
				Glyph(11088, 8), // m
				Glyph(11232, 8), // n
				Glyph(11376, 8), // o
				Glyph(11520, 8), // p
				Glyph(11664, 8), // q
				Glyph(11808, 8), // r
				Glyph(11952, 8), // s
				Glyph(12096, 8), // t
				Glyph(12240, 8), // u
				Glyph(12384, 8), // v
				Glyph(12528, 8), // w
				Glyph(12672, 8), // x
				Glyph(12816, 8), // y
				Glyph(12960, 8), // z
				Glyph(13104, 8), // {
				Glyph(13248, 8), // |
				Glyph(13392, 8), // }
				Glyph(13536, 8) // ~
			};

			const uint8_t _bitmap[1710] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x7F, 0x36, 0x36, 0x36,
				0x7F, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x3C, 0x66, 0x06, 0x0C,
				0x18, 0x30, 0x60, 0x66, 0x3C, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x63, 0x33,
				0x33, 0x18, 0x18, 0x0C, 0x0C, 0x66, 0x66, 0x63, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C,
				0x36, 0x36, 0x1C, 0x0C, 0x5E, 0x7B, 0x33, 0x33, 0x33, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x18, 0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x30, 0x18, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x18, 0x30, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18,
				0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x66,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x7E, 0x18,
				0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x1C, 0x18, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
				0xC0, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x0C, 0x06, 0x06, 0x03, 0x03, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x1C, 0x36, 0x63, 0x63, 0x73, 0x6B, 0x67, 0x63, 0x63, 0x36, 0x1C, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1C, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x60, 0x30, 0x18, 0x0C, 0x06,
				0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x60, 0x60, 0x38, 0x60,
				0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x38, 0x3C, 0x36, 0x33,
				0x33, 0x7F, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x06, 0x06,
				0x06, 0x3E, 0x60, 0x60, 0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
				0x0C, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x7E, 0x60, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x6E, 0x3C, 0x76, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x30, 0x1C,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18,
				0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00,
				0x00, 0x1C, 0x18, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C,
				0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x7E, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x3C, 0x66, 0x66, 0x60, 0x30, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x3E, 0x63, 0x63, 0x63, 0x7B, 0x7B, 0x7B, 0x3B, 0x03, 0x03, 0x3E, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x66,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x66,
				0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x06, 0x06, 0x06,
				0x06, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x36, 0x66, 0x66, 0x66,
				0x66, 0x66, 0x66, 0x66, 0x36, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x06, 0x06,
				0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E,
				0x06, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x3C, 0x66, 0x66, 0x06, 0x06, 0x76, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x66,
				0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x33, 0x33, 0x1B, 0x0F, 0x1B,
				0x33, 0x33, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06,
				0x06, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x77, 0x77,
				0x7F, 0x6B, 0x6B, 0x63, 0x63, 0x63, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63,
				0x63, 0x67, 0x67, 0x6F, 0x7F, 0x7B, 0x73, 0x73, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C,
				0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x66,
				0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x06, 0x0C, 0x18, 0x30,
				0x60, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18,
				0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
				0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66,
				0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x63, 0x63, 0x63, 0x63, 0x63, 0x6B, 0x6B, 0x7F, 0x77, 0x77, 0x63, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x3C, 0x66, 0xC3, 0xC3, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x60, 0x60, 0x30, 0x30, 0x18, 0x0C, 0x0C, 0x06,
				0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
				0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x06, 0x06, 0x0C, 0x0C, 0x18,
				0x18, 0x30, 0x30, 0x60, 0x60, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30,
				0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C,
				0x36, 0x36, 0x63, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
				0x00, 0x00, 0x18, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x7C,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66,
				0x66, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06,
				0x06, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x7C,
				0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x3C, 0x66, 0x66, 0x7E, 0x06, 0x06, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
				0x0C, 0x0C, 0x0C, 0x7E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x3E, 0x00,
				0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30,
				0x30, 0x30, 0x30, 0x30, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x36,
				0x1E, 0x36, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18,
				0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x33, 0x7F, 0x6B, 0x6B, 0x6B, 0x6B, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x06,
				0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C,
				0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x06, 0x06,
				0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x06, 0x06,
				0x3C, 0x60, 0x60, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x7E,
				0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x6B, 0x6B, 0x6B, 0x3E, 0x36, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x63, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C,
				0x60, 0x60, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x60, 0x30, 0x18, 0x0C,
				0x06, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0F, 0x18,
				0x18, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
				0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x0C, 0x0C,
				0x0C, 0x0C, 0x78, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x6B,
				0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			};
	};
}