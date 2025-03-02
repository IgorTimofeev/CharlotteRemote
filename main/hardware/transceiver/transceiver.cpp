#include "transceiver.h"
#include "esp_log.h"
#include "mbedtls/aes.h"
#include "../../rc.h"

namespace pizda {
	bool Transceiver::_canOperate = true;

	void Transceiver::onDio1Action() {
		_canOperate = true;
	}

	void Transceiver::setup() {
		ESP_LOGI("Transceiver", "Initializing");

		auto state = _sx1262.begin(
			constants::transceiver::frequency,
			constants::transceiver::bandwidth,
			constants::transceiver::spreadingFactor,
			constants::transceiver::codingRate,
			RADIOLIB_SX126X_SYNC_WORD_PRIVATE,
			constants::transceiver::power,
			constants::transceiver::preambleLength,
			0,
			false
		);

		if (state != RADIOLIB_ERR_NONE) {
			ESP_LOGE("Transceiver", "Failure, code: %d", state);

			while (true)
				vTaskDelay(100 / portTICK_PERIOD_MS);
		}

		_sx1262.setDio1Action(onDio1Action);

		_mode = TransceiverMode::StartReceive;
	}

	void Transceiver::tick() {
		if (!_canOperate || esp_timer_get_time() < _deadline)
			return;

		switch (_mode) {
			case Transmit:
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

				break;

			case StartReceive:
				startReceive();
				break;

			case Receive:
				receive();
				break;
		}

		_deadline = esp_timer_get_time() + constants::transceiver::tickInterval;
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
		_mode = TransceiverMode::Receive;
		_canOperate = false;

		auto state = _sx1262.startReceive();

		if (state != RADIOLIB_ERR_NONE) {
			ESP_LOGE("Transceiver", "startReceive() failed with code: %d", state);
		}
	}

	void Transceiver::receive() {
		_mode = TransceiverMode::StartReceive;

		// Putting transceiver to standby mode while reading
		auto state = _sx1262.standby();

		if (state == RADIOLIB_ERR_NONE) {
			// Checking if we have packet
			auto packetLength = _sx1262.getPacketLength();

			if (packetLength > 0) {
				// Reading data
				state = _sx1262.readData(_sx1262Buffer, packetLength);

				if (state == RADIOLIB_ERR_NONE) {
					// Checking header
					auto header = ((uint32_t *) &_sx1262Buffer)[0];
					uint8_t headerLength = sizeof(constants::transceiver::packetHeader);

					if (header == constants::transceiver::packetHeader) {
						uint8_t encryptedLength = packetLength - headerLength;

						// Decrypting
						auto aes = esp_aes_context();
						esp_aes_init(&aes);
						esp_aes_setkey(&aes, _aesKey, sizeof(_aesKey) * 8);

						memcpy(_aesIVCopy, _aesIV, sizeof(_aesIV));

						auto decryptState = esp_aes_crypt_cbc(
							&aes,
							ESP_AES_DECRYPT,
							encryptedLength,
							_aesIVCopy,
							(uint8_t *) &_sx1262Buffer + headerLength,
							_aesBuffer
						);

						esp_aes_free(&aes);

						if (decryptState == 0) {
							_rssi = _sx1262.getRSSI();
							_snr = _sx1262.getSNR();

							parsePacket(_aesBuffer);
						}
						else {
							ESP_LOGE("Transceiver", "Decrypting failed: %d\n", encryptedLength);
						}
					}
					else {
						ESP_LOGE("Transceiver", "Unsupported header: %02lX\n", header);
					}
				}
				else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
					ESP_LOGE("Transceiver", "readData() failed, CRC error");
				}
				else {
					ESP_LOGE("Transceiver", "readData() failed with code %d\n", state);
				}
			} else {
				ESP_LOGE("Transceiver", "Got zero packet length");
			}
		}
		else {
			ESP_LOGE("Transceiver", "standby() failed with code %d\n", state);
		}
	}

	void Transceiver::parsePacket(uint8_t *bufferPtr) {
		auto& rc = RC::getInstance();
		auto packetType = (PacketType) *bufferPtr;
		bufferPtr += sizeof(PacketType);

		switch (packetType) {
			case PacketType::aircraftAHRS:
				{
					auto ahrsPacket = (AircraftAHRSPacket*) bufferPtr;
					auto& remoteData = rc.getRemoteData();

//					ahrsPacket->print();

					remoteData.setPitch(ahrsPacket->pitch);
					remoteData.setRoll(ahrsPacket->roll);
					remoteData.setYaw(ahrsPacket->yaw);

					remoteData.setTemperature(ahrsPacket->temperature);
					remoteData.setPressure(ahrsPacket->pressure);

					remoteData.setAltitude(ahrsPacket->altitude);
					remoteData.setSpeed(ahrsPacket->speed);
				}

				break;

			default:
				ESP_LOGE("Transceiver", "Unknown packet type: %d\n", (uint8_t) packetType);

				break;
		}
	}

	float Transceiver::getRssi() const {
		return _rssi;
	}

	float Transceiver::getSnr() const {
		return _snr;
	}
}