#include "axis.h"

#include <limits>
#include <esp_timer.h>
#include <algorithm>
#include "rc.h"

namespace pizda {
	Axis::Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, SettingsAxisData* settings) : _channel(channel), _unitHandle(unitHandle), _settings(settings) {

	}

	void Axis::setup() const {
		constexpr adc_oneshot_chan_cfg_t channelConfig = {
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

		const auto& settings = RC::getInstance().getSettings();

		// Skipping insignificant ADC oscillations
		if (std::abs(rawValue - _rawValue) < settings.axis.jitteringCutoffValue)
			return;

		// Applying low pass filter for buttery smooth landings
		// old * (1 - a) + new * a
		_rawValue =
			_rawValue == 0xFFFF
			? rawValue
			: _rawValue * (0xFFFF - settings.axis.lowPassFactor) / 0xFFFF + rawValue * settings.axis.lowPassFactor / 0xFFFF;

		_processedValue = (std::clamp(_rawValue, _settings->from, _settings->to) - _settings->from) * maxValue / (_settings->to - _settings->from);
	}

	uint16_t Axis::getRawValue() const {
		return _rawValue;
	}

	uint16_t Axis::getValue() const {
		return _processedValue;
	}

	float Axis::getValueF() const {
		return static_cast<float>(getValue()) / static_cast<float>(maxValue);
	}

	uint8_t Axis::getValueUint8() const {
		return getValue() * 0xFF / maxValue;
	}

	uint16_t Axis::getValueUint16() const {
		return getValue() * 0xFFFF / maxValue;
	}

	SettingsAxisData* Axis::getSettings() const {
		return _settings;
	}
}