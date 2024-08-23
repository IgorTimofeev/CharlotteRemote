
#pragma once

#include "Arduino.h"
#include <Ra01S.h>

#define RF_FREQUENCY                                915000000 // Hz  center frequency
#define TX_OUTPUT_POWER                             22        // dBm tx output power
#define LORA_BANDWIDTH                              4         // bandwidth
// 2: 31.25Khz
// 3: 62.5Khz
// 4: 125Khz
// 5: 250KHZ
// 6: 500Khz
#define LORA_SPREADING_FACTOR                       7         // spreading factor [SF5..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
//  2: 4/6,
//  3: 4/7,
//  4: 4/8]

#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_PAYLOADLENGTH                          0         // 0: Variable length packet (explicit header)
// 1..255  Fixed length packet (implicit header)


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

			auto state = _radio.begin(RF_FREQUENCY, TX_OUTPUT_POWER);

			if (state == ERR_NONE) {

			}
			else {
				Serial.print("[SX1262] Failure, code: ");
				Serial.println(state);

				while (true)
					delay(100);
			}

			_radio.LoRaConfig(
				LORA_SPREADING_FACTOR,
				LORA_BANDWIDTH,
				LORA_CODINGRATE,
				LORA_PREAMBLE_LENGTH,
				LORA_PAYLOADLENGTH,
				true,               //crcOn
				false);             //invertIrq
		}

		void tick() {
			if (millis() < _deadline)
				return;

			_receive();

			_deadline = millis() + 1000;
		}

	private:
		SX126x _radio = SX126x(
			4,               //Port-Pin Output: SPI select
			33,               //Port-Pin Output: Reset
			32                //Port-Pin Input:  Busy
		);

		uint32_t _deadline = 0;

		void _receive() {
			uint8_t buffer[255];

			auto len = _radio.Receive(buffer, 255);

			if (len > 0) {
				// packet was successfully received
				Serial.println("KAKOITA PAKET");

				int8_t rssi, snr;
				_radio.GetPacketStatus(&rssi, &snr);

				// print the RSSI (Received Signal Strength Indicator)
				// of the last received packet
				Serial.print("[SX1262] RSSI: ");
				Serial.print(rssi);
				Serial.println(" dBm");

				// print the SNR (Signal-to-Noise Ratio)
				// of the last received packet
				Serial.print("[SX1262] SNR: ");
				Serial.print(snr);
				Serial.println(" dB");

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
			} else {
				Serial.print("Not received");
			}
		}
};