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
		float latitude;
		float longitude;

		float pitch;
		float yaw;
		float roll;
		float slipAndSkid;

		float altitude;
		float speed;

		float pressure;
		float temperature;

		void log() const {
			ESP_LOGI("AircraftPacket", "Latitude, longitude: %f, %f", latitude, longitude);
			ESP_LOGI("AircraftPacket", "Pitch, roll, yaw, slip&skid: %f, %f, %f, %f", pitch, roll, yaw, slipAndSkid);
			ESP_LOGI("AircraftPacket", "Altitude, speed: %f, %f", altitude, speed);
			ESP_LOGI("AircraftPacket", "Temperature, pressure: %f, %f", temperature, pressure);
		}
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