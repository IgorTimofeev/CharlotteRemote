#include "debugOverlay.h"
#include "../rc.h"
#include <esp_private/esp_clk.h>

namespace pizda {
	void DebugOverlay::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		int32_t y = 0;
		std::wstring text;

		const auto totalDeltaTime = rc.getTickDeltaTime();

		const auto renderLine = [renderer, &y, &text](const std::function<void()>& textSetter, const Color* color = &Theme::purple, uint8_t scale = 1) {
			textSetter();

			renderer->renderString(Point(10, y), &Theme::fontNormal, color, text, scale);

			y += Theme::fontNormal.getHeight(scale) + 2;
		};

		// Big fucking FPS counter
		renderLine(
			[&totalDeltaTime, &text]() {
				text = std::format(L"{}", totalDeltaTime > 0 ? 1000 / totalDeltaTime : 0);
			},
			&Theme::yellow,
			3
		);

		renderLine([&text]() {
			text = std::format(L"CPU clock: {} MHz", ((uint32_t) esp_clk_cpu_freq() / 1000000UL));
		});

		renderLine([&text]() {
//			multi_heap_info_t info;
//			heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
//			const auto totalHeap = info.total_free_bytes + info.total_allocated_bytes;
//
//			stream
//				<< L"Heap: "
//				<< (info.total_free_bytes / 1024)
//				<< L" kB / "
//				<< (totalHeap / 1024)
//				<< L" kB, "
//				<< (info.total_free_bytes / totalHeap)
//				<< L"%";

			text = std::format(L"Heap: {} kB", esp_get_free_heap_size() / 1024);
		});

		const auto renderTimeLine = [&renderLine, &totalDeltaTime, &text](std::wstring_view key, uint32_t time) {
			renderLine([time, &key, &totalDeltaTime, &text]() {
				text = std::format(L"{}: {} ms, {}%", key, time, totalDeltaTime > 0 ? time * 100 / totalDeltaTime : 0);
			});
		};

		renderTimeLine(L"Peripherals", rc.getApplication().getPeripheralsDeltaTime());
		renderTimeLine(L"Tick", rc.getApplication().getTickDeltaTime());
		renderTimeLine(L"Layout", rc.getApplication().getLayoutDeltaTime());
		renderTimeLine(L"Render", rc.getApplication().getRenderDeltaTime());
		renderTimeLine(L"Flush", rc.getApplication().getFlushDeltaTime());

		renderLine([&totalDeltaTime, &text]() {
			text = std::format(L"Total: {} ms", totalDeltaTime);
		});
	}
}