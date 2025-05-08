#pragma once

#include "esp_adc/adc_oneshot.h"
#include "../settings.h"

namespace pizda {
	class Axis {
		public:
			Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, SettingsAxisData* settings);

			constexpr static uint16_t maxValue = 4096;

			void setup() const;
			void tick();

			SettingsAxisData* getSettings() const;

			uint16_t getRawValue() const;
			uint16_t getProcessedValue() const;
			float getMappedFloatValue() const;
			uint8_t getMappedUint8Value() const;
			uint16_t getMappedUint16Value() const;

		private:
			adc_channel_t _channel;
			adc_oneshot_unit_handle_t* _unitHandle;
			SettingsAxisData* _settings;

			uint16_t _rawValue = 0xFFFF;
			uint16_t _processedValue = 0;

	};
}