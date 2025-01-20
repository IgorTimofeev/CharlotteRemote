#include "debugOverlay.h"
#include "rc.h"
#include <sstream>

namespace pizdanc {
	void DebugOverlay::onRender(Renderer* renderer) {
		auto& rc = RC::getInstance();

		int32_t y = 0;
		static std::wstringstream stream;

		const auto renderLine = [renderer, &y](std::function<void()> streamWriter) {
			stream.str(std::wstring());
			streamWriter();

			renderer->renderString(Point(0, y), &Theme::fontNormal, &Theme::purple, stream.str());

			y += Theme::fontNormal.getHeight() + 2;
		};
		
		renderLine([]() {
			stream
				<< L"Heap: "
				<< yoba::round((float) ESP.getFreeHeap() / 1024.f, 2)
				<< L" kB / "
				<< yoba::round((float) ESP.getHeapSize() / 1024.f, 2)
				<< L" kB";
		});

		const auto renderTimeLine = [&renderLine, &rc](const std::wstring_view& key, uint32_t time) {
			renderLine([&rc, time, &key]() {
				stream
					<< key
					<< L": "
					<< time
					<< L" ms, "
					<< (time * 100 / rc.getTickDeltaTime())
					<< L"%";
			});
		};

		renderTimeLine(L"Tick", rc.getApplication().getTickDeltaTime());
		renderTimeLine(L"Layout", rc.getApplication().getLayoutDeltaTime());
		renderTimeLine(L"Render", rc.getApplication().getRenderDeltaTime());
		renderTimeLine(L"Flush", rc.getApplication().getFlushDeltaTime());

		renderLine([&rc]() {
			const auto sum =
				rc.getApplication().getTickDeltaTime()
				+ rc.getApplication().getLayoutDeltaTime()
				+ rc.getApplication().getRenderDeltaTime()
				+ rc.getApplication().getFlushDeltaTime();

			stream
			<< L"Other: "
			<< (sum > rc.getTickDeltaTime() ? rc.getTickDeltaTime() - sum : 0)
			<< L" ms";
		});

		renderLine([&rc]() {
			stream
			<< L"Total: "
			<< rc.getTickDeltaTime()
			<< L" ms, "
			<< (1000 / rc.getTickDeltaTime())
			<< L" FPS";
		});
	}
}