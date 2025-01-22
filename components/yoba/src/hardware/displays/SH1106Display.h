#pragma once

#include <cstdint>
#include "SPIDisplay.h"
#include "../../rendering/targets/directRenderTarget.h"
#include "contrastDisplay.h"
#include "invertibleDisplay.h"
#include "main/size.h"

namespace yoba::hardware {
	class SH1106Display : public SPIDisplay, public DirectRenderTarget, public ContrastDisplay, public InvertibleDisplay {
		public:
			// Arduino: 80 MHz
			// ESP-IDF: to be investigated
			SH1106Display(
				uint8_t mosiPin,
				uint8_t misoPin,
				uint8_t sckPin,
				uint8_t csPin,
				uint8_t dcPin,
				int8_t rstPin,
				uint32_t SPIFrequency
			);

			void writePixels(uint8_t* buffer) override;
			void setContrast(uint8_t value) override;
			void setInverted(bool value) override;

		protected:
			void writeSetupCommands() override;
			void writeOrientationChangeCommand() override;
			void writeColorModeChangeCommands() override;

		private:
			static const uint8_t pageCount = 8;

			enum class Command : uint8_t {
				setContrast = 0x81,
				displayAllOnResume = 0xA4,
				displayAllOn = 0xA5,
				normalDisplay = 0xA6,
				invertDisplay = 0xA7,
				displayOff = 0xAE,
				displayOn = 0xAF,
				outputFollowsRam = 0xA4,

				setDisplayOffset = 0xD3,
				setComPins = 0xDA,

				setVComDetect = 0xDB,

				setDisplayClockDiv = 0xD5,
				setPrecharge = 0xD9,

				setMultiplex = 0xA8,

				setLowColumn = 0x00,
				setHighColumn = 0x10,
				setSegmentRemap = 0xA1,

				setStartLine = 0x40,

				memoryMode = 0x20,
				columnAddress = 0x21,
				pageAddress = 0x22,
				setPageAddress = 0xB0,
				setColumnAddressLow = 0x02,
				setColumnAddressHigh = 0x10,

				comScanInc = 0xC0,
				comScanDec = 0xC8,

				segremap = 0xA0,

				chargePump = 0x8D,
			};
	};
}