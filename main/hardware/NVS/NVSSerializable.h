#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>
#include <esp_log.h>

#include "NVSStream.h"

namespace pizda {
	class NVSSerializable {
		public:
			virtual ~NVSSerializable() = default;

			void read() {
				NVSStream stream {};
				stream.openForReading(getNVSNamespace());

				onRead(stream);

				stream.close();
			}

			void write() {
				NVSStream stream {};
				stream.openForWriting(getNVSNamespace());

				onWrite(stream);

				stream.commit();
				stream.close();
			}

			void enqueueWrite() {
				xTaskCreate(enqueueWriteTaskFunction, "NVSSerWrite", 1024, this, 1, nullptr);
			}

		protected:
			virtual const char* getNVSNamespace() = 0;
			virtual void onRead(const NVSStream& stream) = 0;
			virtual void onWrite(const NVSStream& stream) = 0;

		private:
			constexpr static uint32_t _writeDelayTicks = pdMS_TO_TICKS(2500);

			static void enqueueWriteTaskFunction(void* arg) {
				const auto instance = static_cast<NVSSerializable*>(arg);

				vTaskDelay(_writeDelayTicks);
				instance->write();

				vTaskDelete(nullptr);
			}
	};
}