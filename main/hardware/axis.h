#pragma once

#include "esp_adc/adc_oneshot.h"
#include "types/settings/settings.h"

namespace pizda {
	class Axis {
		public:
			Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, bool invertInput, AxisSettingsData* settings);
			
			constexpr static uint8_t valueLengthBits = 12;
			constexpr static uint16_t valueMax = (1 << valueLengthBits) - 1;
			
			constexpr static uint8_t sensitivityMax = 0xFF;

			void setup();
			void read();

			AxisSettingsData* getSettings() const;
			
			uint16_t applySensitivityFilter(uint16_t rawValue);
			
			uint16_t getRawValue() const;
			uint16_t getFilteredValue() const;
			uint8_t getFilteredValueUint8() const;
			float getFilteredValueFloat() const;

		private:
			adc_channel_t _channel;
			adc_oneshot_unit_handle_t* _unitHandle;
			bool _invertInput;
			AxisSettingsData* _settings;

			uint16_t _rawValue = 0xFFFF;
			uint16_t _filteredValue = 0;

	};
}