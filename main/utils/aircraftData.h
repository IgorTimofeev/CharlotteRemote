#pragma once

#include <cstdint>
#include <YOBA/main.h>

#include "types.h"

namespace pizda {
	using namespace YOBA;
	
	class AircraftDataRawAutopilot {
		public:
			float pitchRad = 0;
			float rollRad = 0;
			
			AutopilotLateralMode lateralMode = AutopilotLateralMode::man;
			AutopilotVerticalMode verticalMode = AutopilotVerticalMode::man;
			
			int16_t altitudeM = 0;
			
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
	
	class AircraftDataRaw {
		public:
			// Kronshtadt airfield in Saint-Petersburg for UI testing
			// ПРИВЕТУЛИ ФЕДИНОЙ ДАМЕ СЕРДЦА
			// Upd 26.12.26: а он подарит ей колечко с солнцем! Никто об этом не узнает, никто
			GeographicCoordinates coordinates = {
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
			
			float batteryVoltageV = 0;
			
			AircraftDataRawLights lights {};
			AircraftDataRawAutopilot autopilot {};
	};
	
	class AircraftDataComputedAutopilot {
		public:
			float pitchRad = 0;
			float rollRad = 0;
	};
	
	class AircraftDataComputed {
		public:
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