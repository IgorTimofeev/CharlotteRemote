﻿#pragma once

#include "main/font.h"

namespace yoba {
	class TeenyTinyPixls8Font : public Font {
		public:
			TeenyTinyPixls8Font() : Font(
				32,
				126,
				10,
				_glyphs,
				_bitmap
			) {
				
			}
	
		private:
			const Glyph _glyphs[95] = {
				Glyph(0, 6), //  
				Glyph(60, 6), // !
				Glyph(120, 6), // "
				Glyph(180, 6), // #
				Glyph(240, 6), // $
				Glyph(300, 6), // %
				Glyph(360, 6), // &
				Glyph(420, 6), // '
				Glyph(480, 6), // (
				Glyph(540, 6), // )
				Glyph(600, 6), // *
				Glyph(660, 6), // +
				Glyph(720, 6), // ,
				Glyph(780, 6), // -
				Glyph(840, 6), // .
				Glyph(900, 6), // /
				Glyph(960, 6), // 0
				Glyph(1020, 6), // 1
				Glyph(1080, 6), // 2
				Glyph(1140, 6), // 3
				Glyph(1200, 6), // 4
				Glyph(1260, 6), // 5
				Glyph(1320, 6), // 6
				Glyph(1380, 6), // 7
				Glyph(1440, 6), // 8
				Glyph(1500, 6), // 9
				Glyph(1560, 6), // :
				Glyph(1620, 6), // ;
				Glyph(1680, 6), // <
				Glyph(1740, 6), // =
				Glyph(1800, 6), // >
				Glyph(1860, 6), // ?
				Glyph(1920, 6), // @
				Glyph(1980, 6), // A
				Glyph(2040, 6), // B
				Glyph(2100, 6), // C
				Glyph(2160, 6), // D
				Glyph(2220, 6), // E
				Glyph(2280, 6), // F
				Glyph(2340, 6), // G
				Glyph(2400, 6), // H
				Glyph(2460, 6), // I
				Glyph(2520, 6), // J
				Glyph(2580, 6), // K
				Glyph(2640, 6), // L
				Glyph(2700, 6), // M
				Glyph(2760, 6), // N
				Glyph(2820, 6), // O
				Glyph(2880, 6), // P
				Glyph(2940, 6), // Q
				Glyph(3000, 6), // R
				Glyph(3060, 6), // S
				Glyph(3120, 6), // T
				Glyph(3180, 6), // U
				Glyph(3240, 6), // V
				Glyph(3300, 6), // W
				Glyph(3360, 6), // X
				Glyph(3420, 6), // Y
				Glyph(3480, 6), // Z
				Glyph(3540, 6), // [
				Glyph(3600, 6), // backslash
				Glyph(3660, 6), // ]
				Glyph(3720, 6), // ^
				Glyph(3780, 6), // _
				Glyph(3840, 6), // `
				Glyph(3900, 6), // a
				Glyph(3960, 6), // b
				Glyph(4020, 6), // c
				Glyph(4080, 6), // d
				Glyph(4140, 6), // e
				Glyph(4200, 6), // f
				Glyph(4260, 6), // g
				Glyph(4320, 6), // h
				Glyph(4380, 6), // i
				Glyph(4440, 6), // j
				Glyph(4500, 6), // k
				Glyph(4560, 6), // l
				Glyph(4620, 6), // m
				Glyph(4680, 6), // n
				Glyph(4740, 6), // o
				Glyph(4800, 6), // p
				Glyph(4860, 6), // q
				Glyph(4920, 6), // r
				Glyph(4980, 6), // s
				Glyph(5040, 6), // t
				Glyph(5100, 6), // u
				Glyph(5160, 6), // v
				Glyph(5220, 6), // w
				Glyph(5280, 6), // x
				Glyph(5340, 6), // y
				Glyph(5400, 6), // z
				Glyph(5460, 6), // {
				Glyph(5520, 6), // |
				Glyph(5580, 6), // }
				Glyph(5640, 6) // ~
			};
	
			const uint8_t _bitmap[713] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x10, 0x04, 0x41, 0x00, 0x04, 0x01, 0x00, 0xDB,
				0xB6, 0x01, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x6D, 0xDF, 0xB6, 0x7D, 0xDB, 0x06, 0x00, 0x1C, 0x37,
				0x10, 0x04, 0x76, 0x1C, 0x00, 0xB0, 0x6D, 0x18, 0x41, 0x0C, 0xDB, 0x06, 0x00, 0xC7, 0x31, 0x7C,
				0xDF, 0xF0, 0x7D, 0x00, 0x40, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x31, 0x0C, 0xC3,
				0x40, 0x10, 0x00, 0x30, 0x0C, 0x04, 0x41, 0x10, 0xC3, 0x00, 0x00, 0x04, 0xF1, 0x11, 0xC4, 0x06,
				0x00, 0x00, 0x00, 0x00, 0xC4, 0xF7, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x10,
				0x00, 0x00, 0x00, 0xC0, 0xF7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x10, 0x00,
				0x80, 0x61, 0x04, 0x41, 0x0C, 0xC3, 0x00, 0x00, 0xDF, 0xB7, 0x6D, 0xDB, 0xF6, 0x7D, 0x00, 0x70,
				0x1C, 0x04, 0x41, 0x10, 0xDF, 0x07, 0x00, 0xDF, 0x87, 0x7D, 0xDF, 0xF0, 0x7D, 0x00, 0xF0, 0x7D,
				0xD8, 0xF7, 0x61, 0xDF, 0x07, 0x00, 0xDB, 0xB6, 0x7D, 0x1F, 0x86, 0x61, 0x00, 0xF0, 0x7D, 0xC3,
				0xF7, 0x61, 0xDF, 0x07, 0x00, 0xDF, 0x37, 0x7C, 0xDF, 0xF6, 0x7D, 0x00, 0xF0, 0x7D, 0x1B, 0x86,
				0x61, 0x18, 0x06, 0x00, 0xDF, 0xB7, 0x7D, 0xDF, 0xF6, 0x7D, 0x00, 0xF0, 0x7D, 0xDB, 0xF7, 0x61,
				0x18, 0x06, 0x00, 0x00, 0x40, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x10, 0x04,
				0x01, 0x00, 0x18, 0x46, 0x0C, 0x03, 0x81, 0x61, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x7C, 0x00, 0x00,
				0x00, 0xC3, 0x40, 0x60, 0x18, 0x31, 0x0C, 0x00, 0xF0, 0x7D, 0x18, 0xC7, 0x01, 0x04, 0x01, 0x00,
				0xDF, 0xB7, 0x6D, 0xDB, 0xF0, 0x7D, 0x00, 0xF0, 0x7D, 0xDB, 0xF7, 0x6D, 0xDB, 0x06, 0x00, 0xDF,
				0xB7, 0x1D, 0xC7, 0xF6, 0x7D, 0x00, 0xF0, 0x7D, 0xC3, 0x30, 0x0C, 0xDF, 0x07, 0x00, 0xC7, 0xB1,
				0x6D, 0xDB, 0x76, 0x1C, 0x00, 0xF0, 0x7D, 0xC3, 0xF7, 0x0D, 0xDF, 0x07, 0x00, 0xDF, 0x37, 0x7C,
				0xDF, 0x30, 0x0C, 0x00, 0xF0, 0x7D, 0xC3, 0xB6, 0x6D, 0xDF, 0x07, 0x00, 0xDB, 0xB6, 0x7D, 0xDF,
				0xB6, 0x6D, 0x00, 0xF0, 0x7D, 0x04, 0x41, 0x10, 0xDF, 0x07, 0x00, 0x18, 0x86, 0x61, 0xD8, 0xF6,
				0x7D, 0x00, 0xB0, 0x6D, 0xDB, 0x71, 0x6C, 0xDB, 0x06, 0x00, 0xC3, 0x30, 0x0C, 0xC3, 0xF0, 0x7D,
				0x00, 0xB0, 0x6D, 0xDF, 0xB6, 0x6D, 0xDB, 0x06, 0x00, 0xDF, 0xB7, 0x6D, 0xDB, 0xB6, 0x6D, 0x00,
				0xF0, 0x7D, 0xDB, 0xB6, 0x6D, 0xDF, 0x07, 0x00, 0xDF, 0xB7, 0x7D, 0xDF, 0x30, 0x0C, 0x00, 0xF0,
				0x7D, 0xDB, 0xB6, 0x1D, 0xDB, 0x06, 0x00, 0xDF, 0xB7, 0x1D, 0xC7, 0xB6, 0x6D, 0x00, 0xF0, 0x7D,
				0xC3, 0xF7, 0x61, 0xDF, 0x07, 0x00, 0xDF, 0x47, 0x10, 0x04, 0x41, 0x10, 0x00, 0xB0, 0x6D, 0xDB,
				0xB6, 0x6D, 0xDF, 0x07, 0x00, 0xDB, 0xB6, 0x6D, 0xDB, 0x46, 0x10, 0x00, 0xB0, 0x6D, 0xDB, 0xB6,
				0x7D, 0xDB, 0x06, 0x00, 0xDB, 0xB6, 0x11, 0xC4, 0xB6, 0x6D, 0x00, 0xB0, 0x6D, 0xDB, 0xF7, 0x11,
				0x04, 0x01, 0x00, 0xDF, 0x87, 0x11, 0xC4, 0xF0, 0x7D, 0x00, 0x70, 0x1C, 0xC3, 0x30, 0x0C, 0xC7,
				0x01, 0x00, 0xC3, 0x40, 0x10, 0x04, 0x86, 0x61, 0x00, 0x70, 0x1C, 0x04, 0x41, 0x10, 0xC7, 0x01,
				0x00, 0x04, 0xB1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDF, 0x07, 0x00,
				0xC3, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0xB6, 0x6D, 0x1C, 0x07, 0x00, 0xC3,
				0xF0, 0x6D, 0xDB, 0xF6, 0x7D, 0x00, 0x00, 0x00, 0xDF, 0x30, 0x0C, 0xDF, 0x07, 0x00, 0x18, 0xF6,
				0x6D, 0xDB, 0xF6, 0x7D, 0x00, 0x00, 0x00, 0xDC, 0xB6, 0x1D, 0x1C, 0x07, 0x00, 0x18, 0x46, 0x7C,
				0x1F, 0x41, 0x10, 0x00, 0x00, 0x00, 0xC7, 0xB6, 0x71, 0xC7, 0x01, 0x00, 0xC3, 0xF0, 0x6D, 0xDB,
				0xB6, 0x6D, 0x00, 0x40, 0x10, 0x00, 0x41, 0x10, 0xDF, 0x07, 0x00, 0x04, 0x01, 0x10, 0x04, 0x71,
				0x1C, 0x00, 0x30, 0x0C, 0xDB, 0x71, 0x6C, 0xDB, 0x06, 0x00, 0xC7, 0x41, 0x10, 0x04, 0xC1, 0x71,
				0x00, 0x00, 0x00, 0xDB, 0xF7, 0x6D, 0xDB, 0x06, 0x00, 0x00, 0xF0, 0x6D, 0xDB, 0xB6, 0x6D, 0x00,
				0x00, 0x00, 0xDF, 0xB6, 0x6D, 0xDF, 0x07, 0x00, 0x00, 0xF0, 0x6D, 0xDB, 0x37, 0x0C, 0x00, 0x00,
				0x00, 0xDF, 0xB6, 0x7D, 0x18, 0x06, 0x00, 0x00, 0xF0, 0x0D, 0xC3, 0x30, 0x0C, 0x00, 0x00, 0x00,
				0xDC, 0x71, 0x70, 0xC7, 0x01, 0x00, 0x00, 0x40, 0x7C, 0x1F, 0xC1, 0x71, 0x00, 0x00, 0x00, 0xDB,
				0xB6, 0x6D, 0xDF, 0x07, 0x00, 0x00, 0xB0, 0x6D, 0xDB, 0x46, 0x10, 0x00, 0x00, 0x00, 0xDB, 0xB6,
				0x7D, 0xDB, 0x06, 0x00, 0x00, 0xB0, 0x11, 0xC4, 0xB6, 0x6D, 0x00, 0x00, 0x00, 0xDB, 0xB6, 0x7D,
				0x18, 0x06, 0x00, 0x00, 0xF0, 0x71, 0xDC, 0xF0, 0x7D, 0x00, 0xC0, 0x71, 0xC4, 0x71, 0x10, 0x1C,
				0x07, 0x00, 0x04, 0x41, 0x10, 0x04, 0x41, 0x10, 0x00, 0x70, 0x1C, 0x04, 0xC7, 0x11, 0xC7, 0x01,
				0x00, 0x00, 0x80, 0x7D, 0xDF, 0x00, 0x00, 0x00, 0x00, 
			};
	};
}