#pragma once

#include "esp_adc/adc_oneshot.h"
#include <settings/settings.h>

namespace pizda {
	class Axis {
		public:
			Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, SettingsAxisData* settings);
			
			constexpr static uint8_t maxValueLengthBits = 12;
			constexpr static uint16_t maxValue = (1 << maxValueLengthBits) - 1;

			void setup() const;
			void tick();

			SettingsAxisData* getSettings() const;

			uint16_t getRawValue() const;
			uint16_t getValue() const;
			float getValueF() const;
			uint8_t getValueUint8() const;
			uint16_t getValueUint16() const;

		private:
			adc_channel_t _channel;
			adc_oneshot_unit_handle_t* _unitHandle;
			SettingsAxisData* _settings;

			uint16_t _rawValue = 0xFFFF;
			uint16_t _processedValue = 0;

	};
}