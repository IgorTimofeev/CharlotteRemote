#include "transceiver.h"
#include "ui/rc_application.h"

namespace pizdanc {
	volatile bool Transceiver::_canOperate = true;

	void Transceiver::onDio1Action() {
		_canOperate = true;
	}

	void Transceiver::begin() {
		Serial.println("[SX1262] Initializing");

		auto state = _sx1262.begin(
			settings::transceiver::frequency,
			settings::transceiver::bandwidth,
			settings::transceiver::spreadingFactor,
			settings::transceiver::codingRate,
			RADIOLIB_SX126X_SYNC_WORD_PRIVATE,
			settings::transceiver::power,
			settings::transceiver::preambleLength,
			0,
			false
		);

		if (state != RADIOLIB_ERR_NONE) {
			Serial.print("[SX1262] Failure, code: ");
			Serial.println(state);

			while (true)
				delay(100);
		}

		_sx1262.setDio1Action(onDio1Action);

		_mode = TransceiverMode::Receive;
	}

	void Transceiver::tick(RCApplication &application) {
		if (!_canOperate || millis() < _deadline)
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

		switch (_mode) {
			case Idle:

				break;
			case Transmit:

				break;

			case StartReceive:
				startReceive();

			case Receive:
				receive(application);

				break;
		}

		_deadline = millis() + settings::transceiver::tickInterval;
	}

	template<typename T>
	void Transceiver::send(PacketType packetType, const T &packet) {
//		//			static_assert(sizeof(AHRSInPacket) <= 256, "Cyka paket balsoi");
//
////		Serial.println("[Transceiver] Encrypting packet");
//
//		auto wrapper = PacketTypeWrapper<T>(packetType, packet);
//
//		uint8_t wrapperLength = sizeof(wrapper);
//		uint8_t encryptedWrapperLength = wrapperLength + 16 - (wrapperLength % 16);
//
//		auto header = settings::transceiver::packetHeader;
//		uint8_t headerLength = sizeof(header);
//		uint8_t totalLength = encryptedWrapperLength + headerLength;
//
//		// Copying header
//		mempcpy(&_AESBuffer, &header, headerLength);
//
//		// Encrypting body
//		auto aes = esp_aes_context();
//		esp_aes_init(&aes);
//		esp_aes_setkey(&aes, _AESKey, sizeof(_AESKey) * 8);
//
//		memcpy(_AESIVCopy, _AESIV, sizeof(_AESIV));
//
//		esp_aes_crypt_cbc(
//			&aes,
//			ESP_AES_ENCRYPT,
//			encryptedWrapperLength,
//			_AESIVCopy,
//			(uint8_t *) &wrapper,
//			(uint8_t *) &_AESBuffer + headerLength
//		);
//
//		esp_aes_free(&aes);
//
////		Serial.printf("[SX1262] Sending packet with type %d of %d bytes\n", packetType, totalLength);
//
//		if (_sx1262.Send(_AESBuffer, totalLength, SX126x_TXMODE_SYNC)) {
//
//		}
//		else {
//			Serial.print("[SX1262] Sending failed");
//		}
	}

	void Transceiver::startReceive() {
		_canOperate = false;

		auto state = _sx1262.startReceive();

		if (state == RADIOLIB_ERR_NONE) {
			_mode = TransceiverMode::Receive;
		}
		else {
			Serial.printf("[Transceiver] startReceive() failed with code: %d\n", state);
		}
	}

	void Transceiver::receive(RCApplication &application) {
		auto packetLength = _sx1262.getPacketLength();

		Serial.printf("[Transceiver] Got packet of %d bytes\n", packetLength);

		if (packetLength > 0) {
			auto state = _sx1262.readData(_sx1262Buffer, packetLength);

			if (state == RADIOLIB_ERR_NONE) {
				Serial.printf("[Transceiver] Got packet of %d bytes\n", packetLength);

				// Checking header
				auto header = ((uint32_t*) &_sx1262Buffer)[0];
				uint8_t headerLength = sizeof(settings::transceiver::packetHeader);

				if (header == settings::transceiver::packetHeader) {
					uint8_t encryptedLength = packetLength - headerLength;

					// Decrypting
					auto aes = esp_aes_context();
					esp_aes_init(&aes);
					esp_aes_setkey(&aes, _AESKey, sizeof(_AESKey) * 8);

					memcpy(_AESIVCopy, _AESIV, sizeof(_AESIV));

					auto decryptState = esp_aes_crypt_cbc(
						&aes,
						ESP_AES_DECRYPT,
						encryptedLength,
						_AESIVCopy,
						(uint8_t*) &_sx1262Buffer + headerLength,
						_AESBuffer
					);

					esp_aes_free(&aes);

					if (decryptState == 0) {
						parsePacket(application, _AESBuffer);
					}
					else {
						Serial.printf("Decrypting failed: %d\n", encryptedLength);
					}
				}
				else {
					Serial.printf("[Transceiver] Unsupported header: %02X\n", header);
				}

				application.getOnboardLed().blink();
			}
			else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
				Serial.println("[Transceiver] Receive failed, CRC error");
			}
			else {
				Serial.print("[Transceiver] Receive failed, code ");
				Serial.println(state);
			}
		}

		_mode = TransceiverMode::StartReceive;
	}

	void Transceiver::parsePacket(RCApplication &application, uint8_t *bufferPtr) {
		auto packetType = (PacketType) *bufferPtr;
		bufferPtr += sizeof(PacketType);

		switch (packetType) {
			case AircraftAHRS:
				{
					auto ahrsPacket = (AircraftAHRSPacket*) bufferPtr;

//					ahrsPacket->print();

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

					application.getWorkspace().invalidate();
				}

				break;

			default:
				Serial.printf("[Transceiver] Unknown packet type: %d\n", packetType);

				break;
		}
	}
}