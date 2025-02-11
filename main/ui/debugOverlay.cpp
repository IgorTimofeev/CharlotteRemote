#include "debugOverlay.h"
#include "../rc.h"
#include <sstream>
#include <esp_private/esp_clk.h>

namespace pizda {
	void DebugOverlay::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		int32_t y = 0;
		static std::wstringstream stream;

		const auto totalDeltaTime = rc.getTickDeltaTime();

		const auto renderLine = [renderer, &y](const std::function<void()>& streamWriter, const Color* color = &Theme::purple, uint8_t scale = 1) {
			stream.str(std::wstring());
			streamWriter();

			renderer->renderString(Point(10, y), &Theme::fontNormal, color, stream.str(), scale);

			y += Theme::fontNormal.getHeight(scale) + 2;
		};

		// Big fucking FPS counter
		renderLine(
			[&totalDeltaTime]() {
				stream << (totalDeltaTime > 0 ? 1000 / totalDeltaTime : 0);
			},
			&Theme::yellow,
			3
		);

		renderLine([]() {
			stream
				<< L"CPU clock: "
				<< ((uint32_t) esp_clk_cpu_freq() / 1000000UL)
				<< L" MHz";
		});

		renderLine([]() {
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

			stream
				<< L"Heap: "
				<< (esp_get_free_heap_size() / 1024)
				<< L" kB";
		});

		const auto renderTimeLine = [&renderLine, &totalDeltaTime](const std::wstring_view& key, uint32_t time) {
			renderLine([time, &key, &totalDeltaTime]() {
				stream
					<< key
					<< L": "
					<< time
					<< L" ms, "
					<< (totalDeltaTime > 0 ? time * 100 / totalDeltaTime : 0)
					<< L"%";
			});
		};

		renderTimeLine(L"Peripherals", rc.getApplication().getPeripheralsDeltaTime());
		renderTimeLine(L"Tick", rc.getApplication().getTickDeltaTime());
		renderTimeLine(L"Layout", rc.getApplication().getLayoutDeltaTime());
		renderTimeLine(L"Render", rc.getApplication().getRenderDeltaTime());
		renderTimeLine(L"Flush", rc.getApplication().getFlushDeltaTime());

		renderLine([&totalDeltaTime]() {
			stream << L"Total: " << totalDeltaTime << L" ms";
		});
	}
}