#pragma once

#include "cstdint"
#include "../../data.h"

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
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct AircraftAHRSPacket {
		float pitch;
		float roll;
		float yaw;

		float temperature;
		float pressure;

		float altitude;
		float speed;

		bool strobeLights;

		void print() const {
			ESP_LOGI("AHRSPacket", " Pitch, roll, yaw: %f, %f, %f\n", pitch, roll, yaw);
			ESP_LOGI("AHRSPacket", " Temperature, pressure: %f, %f\n", temperature, pressure);
			ESP_LOGI("AHRSPacket", " Altitude, speed: %f, %f\n", altitude, speed);
			ESP_LOGI("AHRSPacket", " Strobe lights: %d\n", strobeLights);
		}
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct ControllerCommandPacket {
		uint8_t throttle;
		uint8_t ailerons;
		uint8_t rudder;
		uint8_t flaps;

		AltimeterMode altimeterMode;
		float altimeterPressure;

		bool strobeLights;
	};
	#pragma pack(pop)
}