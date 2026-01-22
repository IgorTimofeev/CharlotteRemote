#pragma once

#include <cstdint>
#include <YOBA/main.h>

#include <esp_timer.h>

#include "types/generic.h"

namespace pizda {
	using namespace YOBA;
	
	class AircraftDataRawAutopilot {
		public:
			float pitchRad = 0;
			float rollRad = 0;

			AutopilotLateralMode lateralMode = AutopilotLateralMode::man;
			AutopilotVerticalMode verticalMode = AutopilotVerticalMode::man;
			
			int16_t targetAltitudeM = 0;
			
			bool autothrottle = false;
			bool autopilot = false;
	};
	
	class AircraftDataRawLights {
		public:
			bool navigation = false;
			bool strobe = false;
			bool landing = false;
			bool cabin = false;
	};
	
	class AircraftDataRawCalibration {
		public:
			AircraftCalibrationSystem system = AircraftCalibrationSystem::accelAndGyro;
			uint8_t progress = 0;
			
			bool isCalibrating() const {
				return _calibrating;
			}
			
			void setCalibrating(const bool state) {
				_calibrating = state;
				_validUntil = _calibrating ? esp_timer_get_time() + 1'000'000 : 0;
			}
			
			void checkValidTime() {
				if (_validUntil == 0 || esp_timer_get_time() < _validUntil)
					return;
				
				_calibrating = false;
				_validUntil = 0;
			}
			
		private:
			bool _calibrating = false;
			int64_t _validUntil = 0;
	};
	
	class AircraftDataRaw {
		public:
			// Kronshtadt airfield in Saint-Petersburg for UI testing
			// ПРИВЕТУЛИ ФЕДИНОЙ ДАМЕ СЕРДЦА
			// Upd 26.12.26: а он подарит ей колечко с солнцем! Никто об этом не узнает, никто
			GeographicCoordinates coordinates {
				toRadians(60.014581566191914f),
				toRadians(29.70258579817704f),
				0
			};
			
			float groundSpeedKt = 0;
			float windSpeed = 0;
			uint8_t throttle_0_255 = 0;
			
			float airspeedMPS = 0;
			float airspeedTrendMPS = 0;
			
			float altitudeTrendM = 0;
			
			float verticalSpeedMPM = 0;
			
			float rollRad = 0;
			float pitchRad = 0;
			float yawRad = 0;
			
			float slipAndSkidFactor = 0;
			
			float flightPathVectorPitchRad = 0;
			float flightPathVectorYawRad = 0;
			
			float windDirectionRad = 0;
			
			uint16_t batteryVoltageMV = 0;
			
			AircraftDataRawLights lights {};
			AircraftDataRawAutopilot autopilot {};
			AircraftDataRawCalibration calibration {};
	};
	
	class AircraftDataComputedAutopilot {
		public:
			float pitchRad = 0;
			float rollRad = 0;
	};
	
	class AircraftDataComputed {
		public:
			GeographicCoordinates coordinates {};

			float airspeedKt = 0;
			float airspeedTrendKt = 0;
			
			float altitudeFt = 0;
			float altitudeTrendFt = 0;
			
			float verticalSpeedFtPM = 0;

			float rollRad = 0;
			float pitchRad = 0;
			float yawRad = 0;
			float headingDeg = 0;
			
			float slipAndSkidFactor = 0;
			
			float flightPathVectorPitchRad = 0;
			float flightPathVectorYawRad = 0;
			
			float windDirectionRad = 0;
			float throttle_0_1 = 0;
			
			AircraftDataComputedAutopilot autopilot {};
	};

	class AircraftData {
		public:
			AircraftDataRaw raw {};
			AircraftDataComputed computed {};
	};
}