#include "axis.h"

#include <limits>
#include <esp_timer.h>
#include <esp_log.h>
#include <algorithm>
#include "../rc.h"

namespace pizda {
	pizda::Axis::Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, pizda::AxisSettings* settings) : _channel(channel), _unitHandle(unitHandle), _settings(settings) {

	}

	void Axis::setup() {
		adc_oneshot_chan_cfg_t channelConfig = {
			.atten = ADC_ATTEN_DB_12,
			.bitwidth = ADC_BITWIDTH_12
		};

		ESP_ERROR_CHECK(adc_oneshot_config_channel(*_unitHandle, _channel, &channelConfig));
	}

	void Axis::tick() {
		int rawValue;
		ESP_ERROR_CHECK(adc_oneshot_read(*_unitHandle, _channel, &rawValue));

		// Inverting axis if required
		if (_settings->inverted)
			rawValue = maxValue - rawValue;

		// Skipping insignificant ADC oscillations
		if (std::abs(rawValue - _rawValue) < 30)
			return;

		// Applying low pass filter for buttery smooth landings
		// old * (1 - a) + new * a
		const auto& settings = RC::getInstance().getSettings();

		_rawValue =
			_rawValue == 0xFFFF
			? rawValue
			: _rawValue * (0xFFFF - settings.axisLowPassFactor) / 0xFFFF + rawValue * settings.axisLowPassFactor / 0xFFFF;

		_processedValue = (std::clamp(_rawValue, _settings->from, _settings->to) - _settings->from) * maxValue / (_settings->to - _settings->from);
	}

	uint16_t Axis::getRawValue() {
		return _rawValue;
	}

	uint16_t Axis::getProcessedValue() {
		return _processedValue;
	}

	float Axis::getProcessedFloatValue() {
		return (float) getProcessedValue() / (float) maxValue;
	}

	AxisSettings* Axis::getSettings() {
		return _settings;
	}
}