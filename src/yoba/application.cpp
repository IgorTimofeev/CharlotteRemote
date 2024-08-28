#include "application.h"
#include "point.h"
#include "event.h"

namespace yoba {
	Application::Application(
		const uint8_t &touchRstPin,
		const uint8_t &touchIntPin
	) :
		_screen(
			Size(TFT_HEIGHT, TFT_WIDTH),
			touchRstPin,
			touchIntPin
		)
	{
		_workspace.setSize(_screen.getResolution());

		// Touch
		_screen.addOnTouchDown([this](const Point& point) {
			auto event = TouchDownEvent(point);
			_workspace.handleEvent(event);
		});

		_screen.addOnTouchDrag([this](const Point& point) {
			auto event = TouchDragEvent(point);
			_workspace.handleEvent(event);
		});

		_screen.addOnTouchUp([this](const Point& point) {
			auto event = TouchUpEvent(point);
			_workspace.handleEvent(event);
		});

		// Pinch
		_screen.addOnPinchDown([this](const Point& point1, const Point& point2) {
			auto event = PinchDownEvent(point1, point2);
			_workspace.handleEvent(event);
		});

		_screen.addOnPinchDrag([this](const Point& point1, const Point& point2) {
			auto event = PinchDragEvent(point1, point2);
			_workspace.handleEvent(event);
		});

		_screen.addOnPinchUp([this](const Point& point1, const Point& point2) {
			auto event = PinchUpEvent(point1, point2);
			_workspace.handleEvent(event);
		});
	}

	void Application::begin(uint8_t colorDepth, const uint16_t* palette) {
		_screen.begin(colorDepth, palette);
	}

	void Application::tick() {
		// Tick
		if (millis() > _tickDeadline) {
			onTick();

			_tickDeadline = millis() + _tickInterval;
		}

		// Render
		if (millis() > _renderDeadline) {
			onRender();

			_renderDeadline = millis() + _renderInterval;
		}
	}

	void Application::onTick() {
		_screen.tick();
		_workspace.tick();
	}

	void Application::onRender() {
		_workspace.measure(_screen);
		_workspace.arrange();
		_workspace.render(_screen);
	}

	uint32_t Application::getTickInterval() const {
		return _tickInterval;
	}

	void Application::setTickInterval(uint32_t tickInterval) {
		_tickInterval = tickInterval;
	}

	uint32_t Application::getRenderInterval() const {
		return _renderInterval;
	}

	void Application::setRenderInterval(uint32_t renderInterval) {
		_renderInterval = renderInterval;
	}
}