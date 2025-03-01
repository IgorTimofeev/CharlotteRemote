#pragma once

#include "esp_adc/adc_oneshot.h"

namespace pizda {
	#pragma pack(push, 1)

	class AxisSettings {
		public:
			uint16_t from = 0;
			uint16_t to = 4096;
			bool inverted = false;
	};

	#pragma pack(pop)

	class Axis {
		public:
			Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, AxisSettings* settings);

			constexpr static uint16_t maxValue = 4096;

			void setup();
			void tick();

			AxisSettings* getSettings();

			uint16_t getRawValue();
			uint16_t getProcessedValue();
			float getProcessedFloatValue();

		private:
			adc_channel_t _channel;
			adc_oneshot_unit_handle_t* _unitHandle;
			AxisSettings* _settings;

			uint16_t _rawValue = 0xFFFF;
			uint16_t _processedValue = 0;
	};
}