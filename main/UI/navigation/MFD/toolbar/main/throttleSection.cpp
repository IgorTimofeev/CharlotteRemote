#include "throttleSection.h"

#include "rc.h"
#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "resources/sounds.h"

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
		const auto& rd = rc.getRemoteData();
		const auto& settings = rc.getSettings();

		// Throttles
		throttleIndicator1.setRemoteValue(rd.raw.throttle_0_255);
		throttleIndicator1.setAircraftValue(static_cast<uint8_t>(ad.computed.throttle_0_1 * 0xFF));
		
		throttleIndicator2.setRemoteValue(throttleIndicator1.getRemoteValue());
		throttleIndicator2.setAircraftValue(throttleIndicator1.getAircraftValue());
	}

	void ThrottleSection::onEventBeforeChildren(Event* event) {
		ToolbarSection::onEventBeforeChildren(event);
		
		if (event->getTypeID() != EncoderValueChangedEvent::typeID || !isFocused())
			return;
		
		const auto rotateEvent = reinterpret_cast<EncoderValueChangedEvent*>(event);
		
		auto& rc = RC::getInstance();

		rc.getRemoteData().raw.throttle_0_255 = addSaturating(rc.getRemoteData().raw.throttle_0_255, rotateEvent->getDPSFactor(60, 1, 10) * 0xFF / 100);
		rc.getAudioPlayer().playFeedback();
		
		invalidate();

		event->setHandled(true);
	}
}