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
		int valueRaw;
		ESP_ERROR_CHECK(adc_oneshot_read(*_unitHandle, _channel, &valueRaw));

		// Inverting axis if required
		if (_settings->inverted)
			valueRaw = maxValue - valueRaw;

		const auto& settings = RC::getInstance().getSettings();

		// Initial measurement
		if (_rawValue == 0xFFFF) {
			_rawValue = valueRaw;
		}
		// Subsequent
		else {
			// Skipping insignificant ADC oscillations
			if (std::abs(valueRaw - _rawValue) < settings.axis.jitteringCutoffValue)
				return;
			
			// Applying low pass filter for buttery smooth landings
			_rawValue = _rawValue * (0xFFFF - settings.axis.lowPassFactor) / 0xFFFF + valueRaw * settings.axis.lowPassFactor / 0xFFFF;
		}
		
		_mappedValue = mapValue(valueRaw);
	}
	
	uint16_t Axis::mapValue(uint16_t rawValue) {
		const auto settingsDelta = _settings->to - _settings->from;
		const auto maxValueDiv2 = maxValue / 2;
		
		auto rawValueClamped = static_cast<int32_t>(std::clamp(rawValue, _settings->from, _settings->to));
		rawValueClamped = rawValueClamped * maxValue / settingsDelta;
		rawValueClamped = rawValueClamped - maxValueDiv2;
		
		// Exponential sensitivity correction formula
		// power = 3 (any odd power of raw value)
		// corrected = raw * (1 - sensitivityFactor) + raw^power * sensitivityFactor
		//
		// but floating point arithmetics sucks! so...
		const auto sensitivityMappedToMaxValue = static_cast<int32_t>(_settings->sensitivity) * maxValue / 0xFF;
		
		const auto pizda =
			// Linear part
			rawValueClamped * (static_cast<int32_t>(maxValue) - sensitivityMappedToMaxValue) / static_cast<int32_t>(maxValue)
			;
		
		return static_cast<uint16_t>(maxValueDiv2 + pizda);
	}

	uint16_t Axis::getRawValue() const {
		return _rawValue;
	}

	uint16_t Axis::getMappedValue() const {
		return _mappedValue;
	}

	uint8_t Axis::getMappedValueUint8() const {
		return getMappedValue() * 0xFF / maxValue;
	}
	
	float Axis::getMappedValueFloat() const {
		return static_cast<float>(getMappedValue()) / static_cast<float>(maxValue);
	}

	SettingsAxisData* Axis::getSettings() const {
		return _settings;
	}
}