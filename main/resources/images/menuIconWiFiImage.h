﻿#pragma once

#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	class MenuIconWiFiImage : public Image {
		public:
			MenuIconWiFiImage() : Image(
				ImageFlags::palette8Bit | ImageFlags::alpha1Bit,
				Size(45, 33),
				_bitmap
			) {
				
			}
		
		private:
			constexpr static uint8_t _bitmap[564] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
				0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08,
				0x10, 0x20, 0x40, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
				0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x08,
				0x10, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02,
				0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x20, 0x40, 0x80, 0x00,
				0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0xC0,
				0x83, 0x07, 0x0F, 0x1E, 0x3C, 0x78, 0xF0, 0xE0, 0xC1, 0x83, 0x07, 0x01, 0x00, 0x00, 0x02, 0x04,
				0x08, 0xF0, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x3C, 0x08, 0x10, 0xE0, 0x41, 0x80, 0x00, 0x01,
				0x02, 0x3C, 0x78, 0x10, 0x20, 0x40, 0x80, 0x00, 0x0F, 0x1E, 0x04, 0x78, 0xF0, 0xE0, 0x41, 0x00,
				0x00, 0x40, 0x80, 0x00, 0x01, 0x1E, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x07, 0x01, 0x02, 0x04,
				0x08, 0x10, 0x20, 0x40, 0x80, 0x07, 0x0F, 0x02, 0x3C, 0x78, 0xF0, 0xE0, 0xC1, 0x83, 0x07, 0x0F,
				0x1E, 0x3C, 0x08, 0x00, 0x00, 0x08, 0x10, 0x20, 0xC0, 0x83, 0x00, 0x01, 0x02, 0x04, 0x08, 0xF0,
				0x20, 0x40, 0x80, 0x07, 0x01, 0x02, 0x04, 0x08, 0xF0, 0xE0, 0x41, 0x80, 0x07, 0x0F, 0x1E, 0x3C,
				0x78, 0x10, 0xE0, 0xC1, 0x83, 0x07, 0x01, 0x00, 0x00, 0x01, 0x02, 0x04, 0x78, 0x10, 0x20, 0x40,
				0x80, 0x00, 0x01, 0x1E, 0x04, 0x08, 0xF0, 0x20, 0x40, 0x80, 0x00, 0x01, 0x1E, 0x3C, 0x08, 0x10,
				0x20, 0xC0, 0x83, 0x07, 0x0F, 0x02, 0x3C, 0x78, 0xF0, 0x20, 0x00, 0x00, 0x20, 0x40, 0x80, 0x00,
				0x01, 0x1E, 0x04, 0x78, 0x10, 0xE0, 0x41, 0x80, 0x00, 0x01, 0x1E, 0x04, 0x08, 0x10, 0x20, 0xC0,
				0x83, 0x07, 0x01, 0x1E, 0x3C, 0x78, 0xF0, 0xE0, 0x41, 0x80, 0x07, 0x0F, 0x1E, 0x04, 0x00, 0x00,
				0x04, 0x08, 0x10, 0x20, 0xC0, 0x83, 0x00, 0x0F, 0x02, 0x3C, 0x08, 0x10, 0x20, 0xC0, 0x83, 0x00,
				0x01, 0x02, 0x04, 0x78, 0xF0, 0x20, 0xC0, 0x83, 0x07, 0x0F, 0x1E, 0x3C, 0x08, 0xF0, 0xE0, 0xC1,
				0x83, 0x00, 0x00, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08, 0xF0, 0x20, 0xC0, 0x83, 0x00, 0x01, 0x02,
				0x04, 0x78, 0x10, 0x20, 0x40, 0x80, 0x07, 0x0F, 0x1E, 0x04, 0x78, 0xF0, 0xE0, 0xC1, 0x83, 0x07,
				0x01, 0x1E, 0x3C, 0x78, 0x10, 0x00, 0x00, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10,
				0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x78, 0xF0, 0xE0, 0xC1, 0x83, 0x07, 0x0F, 0x1E, 0x3C,
				0x78, 0xF0, 0xE0, 0xC1, 0x83, 0x07, 0x01, 0x00, 0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x01,
				0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
				0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08,
				0x10, 0x20, 0x40, 0x80, 0x00, 0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x80,
				0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x40,
				0x80, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
			};
	};
}