#include "throttleIndicatorLayout.h"

#include "rc.h"

namespace pizda {
	ThrottleIndicatorLayout::ThrottleIndicatorLayout() : InstrumentIndicatorLayout(L"THR") {
		row.setSpacing(6);
		row.setMargin(Margin(0, 5, 0, 0));
		
		throttleIndicator1.setSize(Size(38, 5));
		row += &throttleIndicator1;

		throttleIndicator2.setSize(throttleIndicator1.getSize());
		row += &throttleIndicator2;

		setContent(&row);
	}

	void ThrottleIndicatorLayout::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		const auto& ad = rc.getAircraftData();
		const auto& settings = rc.getSettings();

		// Throttles
		throttleIndicator1.setRemoteValue(settings.controls.throttle);
		throttleIndicator1.setAircraftValue(ad.raw.throttle);
		
		throttleIndicator2.setRemoteValue(throttleIndicator1.getRemoteValue());
		throttleIndicator2.setAircraftValue(throttleIndicator1.getAircraftValue());
	}

	void ThrottleIndicatorLayout::onEventBeforeChildren(Event* event) {
		InstrumentIndicatorLayout::onEventBeforeChildren(event);

		if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
			if (isFocused()) {
				const auto rotateEvent = reinterpret_cast<EncoderValueChangedEvent*>(event);

//				ESP_LOGI("Encoder", "RPS: %ld", rotateEvent->getDPS());

				auto& settings = RC::getInstance().getSettings();
				settings.controls.throttle = addSaturating(settings.controls.throttle, rotateEvent->getDPSFactor(60, 1, 10) * 0xFF / 100);
				settings.controls.scheduleWrite();

				invalidate();

				event->setHandled(true);
			}
		}
		else if (event->getTypeID() == PushButtonEncoderDownEvent::typeID) {
			if (isFocused()) {
				auto& settings = RC::getInstance().getSettings();
				settings.autopilot.autoThrottle = !settings.autopilot.autoThrottle;
				settings.autopilot.scheduleWrite();

				invalidate();

				event->setHandled(true);
			}
		}
	}
}