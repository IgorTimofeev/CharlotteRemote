#pragma once

#include "cstdint"

namespace pizda {
	#pragma pack(push, 1)
	
	enum class PacketType : uint8_t {
		RemoteChannelDataStructure,
		RemoteChannelData,
		RemoteMotorConfiguration,
		
		AircraftAHRS
	};
	
	class Packet {
		public:
			constexpr static uint8_t headerByteCount = 2;
			constexpr static const char* header = "cy";
			
			constexpr static uint8_t typeBitCount = 4;
		
		private:
		
	};
	
	struct AircraftPacket {
		float latitudeRad = 0;
		float longitudeRad = 0;
		float altitudeM = 0;

		float pitchRad = 0;
		float yawRad = 0;
		float rollRad = 0;

		float airSpeedMs = 0;
		float groundSpeedMs = 0;

		float flightPathPitchRad = 0;
		float flightPathYawRad = 0;

		float flightDirectorPitchRad = 0;
		float flightDirectorYawRad = 0;

		uint16_t slipAndSkid = 0;

		uint16_t windDirectionDeg = 0;
		float windSpeedMs = 0;
	};

	struct RemotePacket {
		uint8_t throttle = 0;

		uint8_t ailerons = 0;
		uint8_t elevator = 0;
		uint8_t rudder = 0;

		uint8_t flaps = 0;
		uint8_t spoilers = 0;

		uint32_t altimeterPressurePa = 0;

		uint16_t autopilotAirspeedMs = 0;
		bool autopilotAutoThrottle = false;

		uint16_t autopilotHeadingDeg = 0;
		bool autopilotHeadingHold = false;

		uint16_t autopilotAltitudeM = 0;
		bool autopilotLevelChange = false;

		bool landingGear = false;
		bool strobeLights = false;
	};

	#pragma pack(pop)
}