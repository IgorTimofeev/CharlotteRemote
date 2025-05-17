#pragma once

#include <cstdint>
#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	class AircraftDataComputed {
		public:
			float airSpeed = 0;
			float airSpeedTrend = 0;
			float altitude = 0;
			float altitudeTrend = 0;
			float verticalSpeed = 0;
			float pitch = 0;
			float roll = 0;
			float yaw = 0;
			float slipAndSkid = 0;
			float flightPathVectorPitch = 0;
			float flightPathVectorYaw = 0;
			float flightDirectorPitch = 0;
			float flightDirectorRoll = 0;
			float windDirection = 0;
			float transceiverRSSI = 0;
	};

	class AircraftData {
		public:
			// Kronshtadt airfield in Saint-Petersburg for UI testing
			// ПРИВЕТУЛИ ФЕДИНОЙ ДАМЕ СЕРДЦА
			GeographicCoordinates geographicCoordinates = GeographicCoordinates(
				toRadians(60.014581566191914f),
				toRadians(29.70258579817704f),
				1000
			);

			float groundSpeed = 0;
			float windSpeed = 0;
			uint8_t throttle = 0;

			float airSpeed = 0;
			float airSpeedTrend = 0;
			float altitude = 0;
			float altitudeTrend = 0;
			float verticalSpeed = 0;
			float pitch = 0;
			float roll = 0;
			float yaw = 0;
			float slipAndSkid = 0;
			float flightPathVectorPitch = 0;
			float flightPathVectorYaw = 0;
			float flightDirectorPitch = 0;
			float flightDirectorRoll = 0;
			float windDirection = 0;

			AircraftDataComputed computed = {};
	};
}