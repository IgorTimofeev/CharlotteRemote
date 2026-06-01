#include "debugOverlay.h"
#include <esp_private/esp_clk.h>
#include "rc.h"

namespace pizda {
	DebugOverlay::DebugOverlay() {
		setVisibleForPointerEvents(false);
	}

	void DebugOverlay::onTick() {
		Element::onTick();

		invalidateRender();
	}

	void DebugOverlay::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		int32_t y = 0;

		const auto totalDeltaTime =
			rc.getApplication().getHIDTickDeltaTime()
			+ rc.getApplication().getTickDeltaTime()
			+ rc.getApplication().getLayoutDeltaTime()
			+ rc.getApplication().getRenderDeltaTime()
			+ rc.getApplication().getFlushDeltaTime();

		const auto renderLine = [&renderer, &y](const std::string_view text, const Color* color = &Theme::magenta1, uint8_t scale = 1) {
			renderer->renderText(Point(10, y), &Theme::fontNormal, color, text, scale);

			y += Theme::fontNormal.getHeight(scale) + 2;
		};

		// Big fucking FPS counter
		renderLine(
			std::to_string(totalDeltaTime > 0 ? 1'000'000 / totalDeltaTime : 0),
			&Theme::yellow,
			3
		);

		renderLine(std::format("CPU clock: {} MHz", static_cast<uint32_t>(esp_clk_cpu_freq()) / 1000000UL));

		renderLine(std::format("Heap free: {} kB", esp_get_free_heap_size() / 1024));

		const auto renderTimeLine = [&renderLine, &totalDeltaTime](std::string_view key, const uint32_t time) {
			renderLine(std::format("{}: {} ms, {}%", key, time / 1000, totalDeltaTime > 0 ? time * 100 / totalDeltaTime : 0));
		};

		renderTimeLine("HID", rc.getApplication().getHIDTickDeltaTime());
		renderTimeLine("Tick", rc.getApplication().getTickDeltaTime());
		renderTimeLine("Layout", rc.getApplication().getLayoutDeltaTime());
		renderTimeLine("Render", rc.getApplication().getRenderDeltaTime());
		renderTimeLine("Flush", rc.getApplication().getFlushDeltaTime());
		renderLine(std::format("Total: {} ms", totalDeltaTime / 1000));

		renderLine(std::format("XCVR PPS RX: {}, TX: {}", rc.getTransceiver().getRXPPS(), rc.getTransceiver().getTXPPS()));
	}
}
