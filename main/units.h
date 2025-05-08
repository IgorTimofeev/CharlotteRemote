#pragma once

#include <string_view>

namespace pizda {
	enum class AltimeterMode : uint8_t {
		QNH,
		QNE
	};

	enum class SpeedUnit : uint8_t {
		meterPerSecond,
		knot
	};

	enum class DistanceUnit : uint8_t {
		meter,
		foot
	};

	enum class PressureUnit : uint8_t {
		pascal,
		hectopascal,
		inHg
	};

	class VSpeed {
		public:
			constexpr VSpeed(std::wstring_view name, const uint32_t value) : _name(name), _value(value) {

			}

			std::wstring_view getName() const {
				return _name;
			}

			uint32_t getValue() const {
				return _value;
			}

		private:
			const std::wstring_view _name;
			const uint32_t _value;
	};

	constexpr float convertSpeed(float value, SpeedUnit fromUnit, SpeedUnit toUnit) {
		switch (fromUnit) {
			case SpeedUnit::meterPerSecond: {
				switch (toUnit) {
					case SpeedUnit::meterPerSecond: return value;
					default: return value / 0.5144444444f;
				}
			}
			default: {
				switch (toUnit) {
					case SpeedUnit::meterPerSecond: return value * 0.5144444444f;
					default: return value;
				}
			}
		}
	}

	constexpr float convertDistance(float value, DistanceUnit fromUnit, DistanceUnit toUnit) {
		switch (fromUnit) {
			case DistanceUnit::meter: {
				switch (toUnit) {
					case DistanceUnit::meter: return value;
					default: return value / 0.3048f;
				}
			}
			default: {
				switch (toUnit) {
					case DistanceUnit::meter: return value * 0.3048f;
					default: return value;
				}
			}
		}
	}

	constexpr float convertPressure(float value, PressureUnit fromUnit, PressureUnit toUnit) {
		switch (fromUnit) {
			case PressureUnit::pascal: {
				switch (toUnit) {
					case PressureUnit::pascal: return value;
					case PressureUnit::hectopascal: return value / 100.f;
					default: return value / 3376.85f;
				}
			}
			case PressureUnit::hectopascal: {
				switch (toUnit) {
					case PressureUnit::pascal: return value * 100.f;
					case PressureUnit::hectopascal: return value;
					default: return value * 100.f / 3376.85f;
				}
			}
			default: {
				switch (toUnit) {
					case PressureUnit::pascal: return value * 3376.85f;
					case PressureUnit::hectopascal: return value * 3376.85f / 100.f;
					default: return value;
				}
			}
		}
	}
}