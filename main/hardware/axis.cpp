#include "axis.h"

#include <limits>
#include <esp_timer.h>
#include <algorithm>
#include "rc.h"

namespace pizda {
	Axis::Axis(adc_oneshot_unit_handle_t* unitHandle, adc_channel_t channel, bool invertInput, AxisSettingsData* settings) :
		_channel(channel),
		_unitHandle(unitHandle),
		_invertInput(invertInput),
		_settings(settings)
	{

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
		
		// Inverting input if required
		if (_invertInput)
			rawValue = valueMax - rawValue;
		
		const auto& settings = RC::getInstance().getSettings();

		// Initial measurement
		if (_rawValue == 0xFFFF) {
			_rawValue = rawValue;
		}
		// Subsequent
		else {
			// Skipping insignificant ADC oscillations
			if (std::abs(rawValue - _rawValue) < settings.axis.jitteringCutoffValue)
				return;
			
			// Applying low pass filter for buttery smooth landings
			_rawValue = _rawValue * (0xFFFF - settings.axis.lowPassFactor) / 0xFFFF + rawValue * settings.axis.lowPassFactor / 0xFFFF;
		}
		
		_mappedValue = mapValue(rawValue);
		
		// Inverting output if required
		if (_settings->invertOutput)
			_mappedValue = valueMax - _mappedValue;
	}
	
	uint16_t Axis::mapValue(uint16_t rawValue) {
		// Exponential sensitivity correction formula
		// power = 3 (any odd power of raw value)
		// corrected = raw * (1 - sensitivityFactor) + raw^power * sensitivityFactor
		
		const auto valueClamped = std::clamp(rawValue, _settings->from, _settings->to);
		const auto valueFactor = (static_cast<float>(valueClamped) - _settings->from) / static_cast<float>(_settings->to - _settings->from) * 2.f - 1.f;
		const auto sensitivityFactor = static_cast<float>(_settings->sensitivity) / static_cast<float>(sensitivityMax);
		
		const auto valueCorrectedFactor =
			valueFactor * (1 - sensitivityFactor)
			+ valueFactor * valueFactor * valueFactor * sensitivityFactor;
		
		return static_cast<uint16_t>(static_cast<float>(valueMax) * ((valueCorrectedFactor + 1.f) / 2.f));
	}

	uint16_t Axis::getRawValue() const {
		return _rawValue;
	}

	uint16_t Axis::getMappedValue() const {
		return _mappedValue;
	}

	uint8_t Axis::getMappedValueUint8() const {
		return getMappedValue() * 0xFF / valueMax;
	}
	
	float Axis::getMappedValueFloat() const {
		return static_cast<float>(getMappedValue()) / static_cast<float>(valueMax);
	}

	AxisSettingsData* Axis::getSettings() const {
		return _settings;
	}
}