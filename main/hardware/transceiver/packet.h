#pragma once

#include "cstdint"

namespace pizda {
	enum class PacketType : uint8_t {
		aircraftAHRS = 1,
		controllerCommand = 2
	};

	#pragma pack(push, 1)

	template<typename T>
	struct PacketTypeWrapper {
		explicit PacketTypeWrapper(PacketType type, T body) :
			type(type),
			body(body)
		{

		}

		PacketType type;
		T body;
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

		int16_t slipAndSkid = 0;

		uint16_t windDirectionDeg = 0;
		float windSpeedMs = 0;
	};

	struct RemotePacket {
		uint16_t throttle1 = 0;
		uint16_t throttle2 = 0;

		uint16_t ailerons = 0;
		uint16_t elevator = 0;
		uint16_t rudder = 0;

		uint16_t flaps = 0;
		uint16_t spoilers = 0;

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