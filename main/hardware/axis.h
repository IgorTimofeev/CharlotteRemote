#pragma once

#include "esp_adc/adc_oneshot.h"
#include "../settings.h"

namespace pizda {
	class Axis {
		public:
			Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, SettingsAxisData* settings);

			constexpr static const uint16_t maxValue = 4096;

			void setup();
			void tick();

			SettingsAxisData* getSettings();

			uint16_t getRawValue();
			uint16_t getProcessedValue();
			float getMappedFloatValue();
			uint16_t getMappedUint16Value();

		private:
			adc_channel_t _channel;
			adc_oneshot_unit_handle_t* _unitHandle;
			SettingsAxisData* _settings;

			uint16_t _rawValue = 0xFFFF;
			uint16_t _processedValue = 0;

	};
}