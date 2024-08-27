#include "transceiver.h"
#include "ui/rc_application.h"

namespace pizdanc {
	void Transceiver::begin() {
		Serial.println("[SX1262] Initializing");

		auto state = _sx1262.begin(
			// Hz  center frequency
			915000000,
			// dBm tx output power
			22
		);

		if (state == ERR_NONE) {

		}
		else {
			Serial.print("[SX1262] Failure, code: ");
			Serial.println(state);

			while (true)
				delay(100);
		}

		_sx1262.LoRaConfig(
			// spreading factor [SF5..SF12]
			7,
			// bandwidth
			// 2: 31.25Khz
			// 3: 62.5Khz
			// 4: 125Khz
			// 5: 250KHZ
			// 6: 500Khz
			4,
			// [1: 4/5,
			//  2: 4/6,
			//  3: 4/7,
			//  4: 4/8]
			1,
			// Same for Tx and Rx
			8,
			// 0: Variable length packet (explicit header)
			// 1..255  Fixed length packet (implicit header)
			0,
			//crcOn
			true,
			//invertIrq
			false
		);
	}

	void Transceiver::tick(RCApplication &application) {
		if (millis() < _deadline)
			return;

//		send(
//			PacketType::ControllerCommand,
//			ControllerCommandPacket{
//				.throttle = 1,
//				.ailerons = 2,
//				.rudder = 3,
//
//				.strobeLights = true
//			}
//		);

		receive(application);

		_deadline = millis() + 1000;
	}

	template<typename T>
	void Transceiver::send(PacketType packetType, const T &packet) {
		//			static_assert(sizeof(AHRSInPacket) <= 256, "Cyka paket balsoi");

		Serial.println("[Transceiver] Encrypting packet");

		auto wrapper = PacketTypeWrapper<T>(packetType, packet);

		uint8_t wrapperLength = sizeof(wrapper);
		uint8_t encryptedWrapperLength = wrapperLength + 16 - (wrapperLength % 16);

		auto header = settings::transceiver::packetHeader;
		uint8_t headerLength = sizeof(header);
		uint8_t totalLength = encryptedWrapperLength + headerLength;

		// Copying header
		mempcpy(&_AESBuffer, &header, headerLength);

		// Encrypting body
		memcpy(_AESIVCopy, _AESIV, sizeof(_AESIV));
		esp_aes_crypt_cbc(
			&_AESContext,
			ESP_AES_ENCRYPT,
			encryptedWrapperLength,
			_AESIVCopy,
			(uint8_t *) &wrapper,
			(uint8_t *) &_AESBuffer + headerLength
		);

		Serial.printf("[SX1262] Sending packet of %d bytes\n", totalLength);

		if (_sx1262.Send((uint8_t *) &_AESBuffer, totalLength, SX126x_TXMODE_SYNC)) {

		}
		else {
			Serial.print("[SX1262] Sending failed");
		}
	}

	void Transceiver::receive(RCApplication &application) {
		uint8_t receivedLength = _sx1262.Receive(_sx1262Buffer, sizeof(_sx1262Buffer));

		if (receivedLength == 0)
			return;

		Serial.printf("[Transceiver] Got packet of %d bytes\n", receivedLength);

		Serial.print("Bytes: ");

		for (uint8_t i = 0; i < receivedLength; i++) {
			Serial.printf("%d ", _sx1262Buffer[i]);
		}

		Serial.println();

		uint8_t headerLength = sizeof(settings::transceiver::packetHeader);
		auto header = ((uint32_t*) &_sx1262Buffer)[0];

		// Checking header
		if (header != settings::transceiver::packetHeader) {
			Serial.printf("[Transceiver] Unsupported header: %02X\n", header);
			return;
		}

		Serial.println("[Transceiver] Decrypting packet");

		uint8_t encryptedLength = receivedLength - headerLength;

		// Decrypting
		memcpy(_AESIVCopy, _AESIV, sizeof(_AESIV));

		if (
			esp_aes_crypt_cbc(
				&_AESContext,
				ESP_AES_DECRYPT,
				encryptedLength,
				_AESIVCopy,
				(uint8_t*) &_sx1262Buffer + headerLength,
				_AESBuffer
			) != 0
		) {
			Serial.printf("Decrypting failed: %d\n", encryptedLength);

			return;
		}

		Serial.print("Bytes: ");

		for (uint8_t i = 0; i < encryptedLength; i++) {
			Serial.printf("%d ", _AESBuffer[i]);
		}

		Serial.println();

		parsePacket(application, _AESBuffer);
	}

	void Transceiver::parsePacket(RCApplication &application, uint8_t *bufferPtr) {
		auto packetType = (PacketType) *bufferPtr;
		bufferPtr += sizeof(PacketType);

		switch (packetType) {
			case AircraftAHRS:
				{
					auto ahrsPacket = (AircraftAHRSPacket*) bufferPtr;

					ahrsPacket->print();

					application.getRemoteData().setThrottle(ahrsPacket->throttle);
					application.getRemoteData().setAilerons(ahrsPacket->ailerons);
					application.getRemoteData().setRudder(ahrsPacket->rudder);
					application.getRemoteData().setFlaps(ahrsPacket->flaps);

					application.getRemoteData().setPitch(ahrsPacket->pitch);
					application.getRemoteData().setRoll(ahrsPacket->roll);
					application.getRemoteData().setYaw(ahrsPacket->yaw);

					application.getRemoteData().setTemperature(ahrsPacket->temperature);
					application.getRemoteData().setPressure(ahrsPacket->pressure);

					application.getRemoteData().setAltimeterMode(ahrsPacket->altimeterMode);
					application.getRemoteData().setAltimeterPressure(ahrsPacket->altimeterPressure);

					application.getRemoteData().setAltitude(ahrsPacket->altitude);
					application.getRemoteData().setSpeed(ahrsPacket->speed);

					application.getRemoteData().setStrobeLights(ahrsPacket->strobeLights);
				}

				break;

			default:
				Serial.printf("[Transceiver] Unknown packet type: %d\n", packetType);

				break;
		}
	}
}