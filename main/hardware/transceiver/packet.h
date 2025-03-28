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
		float latitudeRad;
		float longitudeRad;
		float altitudeM;

		float pitchRad;
		float yawRad;
		float rollRad;

		float airSpeedMs;
		float groundSpeedMs;

		float flightPathPitchRad;
		float flightPathYawRad;

		int16_t slipAndSkid;

		float windDirectionRad;
		float windSpeedMs;
	};

	struct RemotePacket {
		uint16_t throttle1;
		uint16_t throttle2;

		uint16_t ailerons;
		uint16_t elevator;
		uint16_t rudder;

		uint16_t flaps;
		uint16_t spoilers;

		bool landingGear;
		bool strobeLights;
	};

	#pragma pack(pop)
}