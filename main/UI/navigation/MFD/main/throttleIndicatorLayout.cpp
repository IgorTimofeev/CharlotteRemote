#include "throttleIndicatorLayout.h"
#include "../../../../rc.h"

namespace pizda {

	ThrottleIndicatorLayout::ThrottleIndicatorLayout() : InstrumentIndicatorLayout(L"THR") {
		row.setOrientation(Orientation::horizontal);
		row.setSpacing(10);

		throttleIndicator1.setWidth(7);
		row += &throttleIndicator1;

		throttleIndicator2.setSize(throttleIndicator1.getSize());
		row += &throttleIndicator2;

		setContent(&row, true);
	}

	void ThrottleIndicatorLayout::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// Throttles
		throttleIndicator1.setRemoteValue(settings.controls.throttle);
		throttleIndicator1.setAircraftValue(rc.getAircraftThrottle());

		throttleIndicator2.setRemoteValue(throttleIndicator1.getRemoteValue());
		throttleIndicator2.setAircraftValue(throttleIndicator1.getAircraftValue());
	}

	void ThrottleIndicatorLayout::onEvent(Event* event) {
		InstrumentIndicatorLayout::onEvent(event);

		if (event->getTypeID() == EncoderRotateEvent::typeID) {
			if (isFocused()) {
				auto rotateEvent = reinterpret_cast<EncoderRotateEvent*>(event);

//				ESP_LOGI("Encoder", "RPS: %ld", rotateEvent->getRPS());

				auto& settings = RC::getInstance().getSettings();
				settings.controls.throttle = YOBA::addSaturating(settings.controls.throttle, rotateEvent->getRPSFactor(60, 1, 10) * 0xFF / 100);
				settings.enqueueWrite();

				invalidate();

				event->setHandled(true);
			}
		}
		else if (event->getTypeID() == EncoderPushEvent::typeID) {
			if (isFocused() && reinterpret_cast<EncoderPushEvent*>(event)->isDown()) {
				auto& settings = RC::getInstance().getSettings();
				settings.autopilot.autoThrottle = !settings.autopilot.autoThrottle;
				settings.enqueueWrite();

				invalidate();

				event->setHandled(true);
			}
		}
	}
}