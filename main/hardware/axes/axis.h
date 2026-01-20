#pragma once

#include <atomic>

#include "esp_adc/adc_oneshot.h"
#include "types/settings/settings.h"

namespace pizda {
	class Axis {
		public:
			constexpr static uint8_t valueLengthBits = 12;
			constexpr static uint16_t valueMax = (1 << valueLengthBits) - 1;
			
			constexpr static uint8_t sensitivityMax = 0xFF;

			void setup(adc_oneshot_unit_handle_t* ADCOneshotUnit, adc_channel_t ADCChannel, bool invertInput, AxisSettingsData* settings);
			void tick();

			AxisSettingsData* getSettings() const;
			
			uint16_t applySensitivityFilter(uint16_t rawValue) const;
			
			uint16_t getRawValue() const;
			uint16_t getFilteredValue() const;
			uint8_t getFilteredValueUint8() const;
			float getFilteredValueFloat() const;

		private:
			adc_oneshot_unit_handle_t* _ADCOneshotUnit = nullptr;
			adc_channel_t _ADCChannel {};
			bool _invertInput = false;
			AxisSettingsData* _settings = nullptr;

			std::atomic<uint16_t> _rawValue = 0xFFFF;
			std::atomic<uint16_t> _filteredValue = 0;

	};
}