#pragma once

#include <cstdint>
#include "screen.h"

namespace yoba {
	class Application {
		public:
			Application(
				const Size &resolution,
				const uint8_t &tftLedPin,
				const uint8_t &touchSdaPin,
				const uint8_t &touchSclPin,
				const uint8_t &touchRstPin,
				const uint8_t &touchIntPin
			);

			virtual void begin();
			virtual void tick();

			virtual void onTick();
			virtual void onRender();

			Screen& getScreen() {
				return _screen;
			}

			const uint16_t &getTps() const {
				return _tps;
			}

			void setTps(const uint16_t &tps) {
				_tps = tps;
			}

			const uint16_t &getFps() const {
				return _fps;
			}

			void setFps(const uint16_t &fps) {
				_fps = fps;
			}

			Workspace &getWorkspace() {
				return _workspace;
			}

		private:
			Workspace _workspace;
			Screen _screen;

			uint16_t _tps = 60;
			uint16_t _fps = 60;

			uint32_t _tickDeadline = 0;
			uint32_t _renderDeadline = 0;
	};
}