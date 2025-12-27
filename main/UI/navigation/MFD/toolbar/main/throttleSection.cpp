#include "throttleSection.h"

#include "rc.h"
#include "UI/navigation/MFD/toolbar/toolbar.h"

namespace pizda {
	ThrottleSection::ThrottleSection() : ToolbarSection(L"THR") {
		row.setSpacing(7);
		ToolbarSection::setDefaultMargin(&row, 6);
		
		row += &throttleIndicator1;
		row += &throttleIndicator2;

		*this += &row;
	}

	void ThrottleSection::onTick() {
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

	void ThrottleSection::onEventBeforeChildren(Event* event) {
		ToolbarSection::onEventBeforeChildren(event);

		if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
			if (isFocused()) {
				const auto rotateEvent = reinterpret_cast<EncoderValueChangedEvent*>(event);
				
				auto& settings = RC::getInstance().getSettings();
				settings.controls.throttle = addSaturating(settings.controls.throttle, rotateEvent->getDPSFactor(60, 1, 10) * 0xFF / 100);
				settings.controls.scheduleWrite();

				invalidate();

				event->setHandled(true);
			}
		}
	}
}