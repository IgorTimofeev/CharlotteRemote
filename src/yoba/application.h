#pragma once

#ifndef YOBA_APPLICATION_H
#define YOBA_APPLICATION_H
#endif

#include <cstdint>
#include "screen.h"

namespace yoba {
	class Application {
		public:
			Application(
				const uint8_t &touchRstPin,
				const uint8_t &touchIntPin
			);

			virtual void begin(uint8_t colorDepth, const uint16_t *palette);
			virtual void tick();

			virtual void onTick();
			virtual void onRender();

			uint32_t getTickInterval() const;

			void setTickInterval(uint32_t tickInterval);

			uint32_t getRenderInterval() const;

			void setRenderInterval(uint32_t renderInterval);

			Screen& getScreen() {
				return _screen;
			}

			Workspace &getWorkspace() {
				return _workspace;
			}

		private:
			Workspace _workspace;
			Screen _screen;

			uint32_t _tickInterval = 1000 / 30;
			uint32_t _renderInterval = 1000 / 30;

			uint32_t _tickDeadline = 0;
			uint32_t _renderDeadline = 0;
	};
}