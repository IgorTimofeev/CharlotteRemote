
#pragma once

#include "Arduino.h"
#include <Ra01S.h>

class Transceiver {
	public:
		Transceiver() = default;

		void begin() {
			//lora.DebugPrint(true);

			auto beginResult = _lora.begin(
				915000000,  //frequency in Hz
				22 //tx power in dBm
			);

			if (beginResult != ERR_NONE) {
				while(1) {
					delay(1);
				}
			}

			_lora.LoRaConfig(
				// Spreading factor
				// SF5..SF12
				7,

				// Bandwidth
				// 2: 31.25Khz
				// 3: 62.5Khz
				// 4: 125Khz
				// 5: 250KHZ
				// 6: 500Khz
				4,

				// Coding rate
				// 1: 4/5,
				// 2: 4/6,
				// 3: 4/7,
				// 4: 4/8
				1,

				// Preamble length, same for Tx and Rx
				8,

				// 0: Variable length packet
				// 1..255: Fixed length packet
				0,

				// crcOn
				true,
				// invertIrq
				false
			);
		}



		void tick() {
			if (millis() < _sendDeadline)
				return;

			_send();
			_receive();
		}

	private:
		uint32_t _sendDeadline = 0;

		SX126x _lora = SX126x(
			4,
			33,
			32
		);

		void _send() {
			char buffer[255];
			sprintf(buffer, "Hello World %lu", millis());
			uint8_t len = strlen(buffer);

			// Wait for transmission to complete
			_lora.Send((uint8_t*) buffer, len, SX126x_TXMODE_ASYNC);

			_sendDeadline = millis() + 1000;
		}


		void _receive() {
			uint8_t buffer[255];
			uint8_t rxLen = _lora.Receive(buffer, 255);

			if (rxLen == 0)
				return;

			Serial.print("Received rxLen:");
			Serial.println(rxLen);

			for(int i=0;i< rxLen;i++) {
				Serial.print(buffer[i], HEX);
				Serial.print(" ");
			}

			Serial.println();

			for(int i=0; i< rxLen; i++) {
				if (buffer[i] > 0x19 && buffer[i] < 0x7F) {
					char myChar = (char) buffer[i];
					Serial.print(myChar);
				}
				else {
					Serial.print("?");
				}
			}

			Serial.println();

			int8_t rssi, snr;
			_lora.GetPacketStatus(&rssi, &snr);
			Serial.print("rssi: ");
			Serial.print(rssi, DEC);
			Serial.println(" dBm");
			Serial.print("snr: ");
			Serial.print(snr, DEC);
			Serial.println(" dB");
		}
};