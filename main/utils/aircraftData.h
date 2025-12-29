#pragma once

#include <cstdint>
#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;
	
	class AircraftDataRaw {
		public:
			// Kronshtadt airfield in Saint-Petersburg for UI testing
			// ПРИВЕТУЛИ ФЕДИНОЙ ДАМЕ СЕРДЦА
			// Upd 26.12.26: а он подарит ей колечко с солнцем! Никто об этом не узнает, никто
			GeographicCoordinates geographicCoordinates = GeographicCoordinates(
				toRadians(60.014581566191914f),
				toRadians(29.70258579817704f),
				1000
			);
			
			float groundSpeedKt = 0;
			float windSpeed = 0;
			uint8_t throttle_0_255 = 0;
			
			float airSpeedMPS = 0;
			float airSpeedTrendMPS = 0;
			
			float altitudeTrendM = 0;
			
			float verticalSpeedMPM = 0;
			
			float rollRad = 0;
			float pitchRad = 0;
			float yawRad = 0;
			
			float slipAndSkidFactor = 0;
			
			float flightPathVectorPitchRad = 0;
			float flightPathVectorYawRad = 0;
			
			float autopilotPitchRad = 0;
			float autopilotRollRad = 0;
			
			float windDirectionRad = 0;
			
			float batteryVoltageV = 0;
	};
	
	class AircraftDataComputed {
		public:
			float airSpeedKt = 0;
			float airSpeedTrendKt = 0;
			
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
			
			float autopilotPitchRad = 0;
			float autopilotRollRad = 0;
			
			float windDirectionRad = 0;
			float throttlePercent01 = 0;
	};

	class AircraftData {
		public:
			AircraftDataRaw raw {};
			AircraftDataComputed computed {};
	};
}