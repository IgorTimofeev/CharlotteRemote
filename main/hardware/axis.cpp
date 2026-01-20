#include "axis.h"

#include <limits>
#include <esp_timer.h>
#include <algorithm>
#include "rc.h"

namespace pizda {
	Axis::Axis(const adc_unit_t ADCUnit, const adc_channel_t ADCChannel, const bool invertInput, AxisSettingsData* settings) :
		_ADCUnit(ADCUnit),
		_ADCChannel(ADCChannel),
		_invertInput(invertInput),
		_settings(settings)
	{

	}

	void Axis::setup() {
		adc_oneshot_unit_init_cfg_t ADC1UnitConfig {};
		ADC1UnitConfig.unit_id = _ADCUnit;
		ADC1UnitConfig.clk_src = ADC_RTC_CLK_SRC_DEFAULT;
		ADC1UnitConfig.ulp_mode = ADC_ULP_MODE_DISABLE;
		ESP_ERROR_CHECK(adc_oneshot_new_unit(&ADC1UnitConfig, &_ADCOneshotUnitHandle));

		adc_oneshot_chan_cfg_t channelConfig {};
		channelConfig.atten = ADC_ATTEN_DB_12;
		channelConfig.bitwidth = ADC_BITWIDTH_12;
		ESP_ERROR_CHECK(adc_oneshot_config_channel(_ADCOneshotUnitHandle, _ADCChannel, &channelConfig));

		read();
	}

	void Axis::read() {
		int rawValue;
		ESP_ERROR_CHECK(adc_oneshot_read(_ADCOneshotUnitHandle, _ADCChannel, &rawValue));
		
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
		
		_filteredValue = applySensitivityFilter(_rawValue);
		
		// Inverting output if required
		if (_settings->invertOutput)
			_rawValue = valueMax - _rawValue;
	}
	
	uint16_t Axis::applySensitivityFilter(const uint16_t rawValue) {
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

	uint16_t Axis::getFilteredValue() const {
		return _filteredValue;
	}

	uint8_t Axis::getFilteredValueUint8() const {
		return getFilteredValue() * 0xFF / valueMax;
	}
	
	float Axis::getFilteredValueFloat() const {
		return static_cast<float>(getFilteredValue()) / static_cast<float>(valueMax);
	}

	AxisSettingsData* Axis::getSettings() const {
		return _settings;
	}
}