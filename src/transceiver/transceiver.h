
#pragma once

#include "Arduino.h"
#include <RadioLib.h>


struct GovnoPacket {
	uint32_t signature;

	float pitch;
	float roll;
	float yaw;

	float temperature;
	float pressure;

	float qnh;
	float altitude;
};

class Transceiver {
	public:
		Transceiver() = default;

		void begin() {
			Serial.println("[SX1262] Initializing");

			auto state = _radio.begin(
				434.0,
				125.0,
				9,
				7,
				RADIOLIB_SX126X_SYNC_WORD_PRIVATE,
				10,
				8,
				0
			);

			if (state == RADIOLIB_ERR_NONE) {

			}
			else {
				Serial.print("[SX1262] Failure, code: ");
				Serial.println(state);

				while (true)
					delay(100);
			}
		}

		void tick() {
			if (millis() < _deadline)
				return;

			_receive();

			_deadline = millis() + 1000;
		}

	private:
		SX1262 _radio = new Module(
			4,
			RADIOLIB_NC,
			33,
			32
		);

		uint32_t _deadline = 0;

		void _receive() {
			uint8_t buffer[255];
			auto state = _radio.receive(buffer, 255);

			if (state == RADIOLIB_ERR_NONE) {
				// packet was successfully received
				Serial.println("KAKOITA PAKET");

				// print the RSSI (Received Signal Strength Indicator)
				// of the last received packet
				Serial.print("[SX1262] RSSI: ");
				Serial.print(_radio.getRSSI());
				Serial.println(" dBm");

				// print the SNR (Signal-to-Noise Ratio)
				// of the last received packet
				Serial.print("[SX1262] SNR: ");
				Serial.print(_radio.getSNR());
				Serial.println(" dB");

				// print frequency error
				Serial.print("[SX1262] Frequency error: ");
				Serial.print(_radio.getSNR());
				Serial.println(" Hz");

				auto* packet = (GovnoPacket*) &buffer;

				// "plane" -> ASCII -> 0x506C416E
				if (packet->signature == 0x506C416E) {
					Serial.println("NORMALI PAKET");

					Serial.print("pitch: ");
					Serial.println(packet->pitch);
					Serial.print("roll: ");
					Serial.println(packet->roll);
					Serial.print("yaw: ");
					Serial.println(packet->yaw);
					Serial.print("temperature: ");
					Serial.println(packet->temperature);
					Serial.print("pressure: ");
					Serial.println(packet->pressure);
					Serial.print("qnh: ");
					Serial.println(packet->qnh);
					Serial.print("altitude: ");
					Serial.println(packet->altitude);
				}
			} else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
				// timeout occurred while waiting for a packet
				Serial.println(F("timeout!"));

			} else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
				// packet was received, but is malformed
				Serial.println(F("CRC error!"));

			} else {
				// some other error occurred
				Serial.print(F("failed, code "));
				Serial.println(state);
			}
		}
};