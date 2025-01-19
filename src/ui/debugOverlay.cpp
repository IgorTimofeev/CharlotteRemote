#include "debugOverlay.h"
#include "rc.h"
#include <sstream>

namespace pizdanc {
	void DebugOverlay::onRender(Renderer* renderer) {

		int32_t y = 0;
		static std::wstringstream stream;

		const auto go = [renderer, &y](std::function<void()> streamWriter) {
			stream.str(std::wstring());
			streamWriter();

			renderer->renderString(Point(0, y), &Theme::fontNormal, &Theme::purple, stream.str());

			y += Theme::fontNormal.getHeight() + 2;
		};

		go([]() {
			auto& rc = RC::getInstance();

			stream << "FPS: ";
			stream << (rc.getTickDeltaTime() == 0 ? 0 : 1000 / rc.getTickDeltaTime());
		});

		go([]() {
			stream << "CPU freq: ";
			stream << ESP.getCpuFreqMHz();
			stream << " MHz";
		});

		go([]() {
			stream << "Heap: ";
			stream << yoba::round((float) ESP.getFreeHeap() / 1024.f, 2);
			stream << " kB / ";
			stream << yoba::round((float) ESP.getHeapSize() / 1024.f, 2);
			stream << " kB";
		});
	}
}