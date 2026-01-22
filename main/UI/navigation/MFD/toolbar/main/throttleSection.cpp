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

		// Throttles
		throttleIndicator1.setRemoteValue(rc.getRemoteData().throttle_0_255);
		throttleIndicator1.setAircraftValue(static_cast<uint8_t>(rc.getAircraftData().computed.throttle_0_1 * 0xFF));
		
		throttleIndicator2.setRemoteValue(throttleIndicator1.getRemoteValue());
		throttleIndicator2.setAircraftValue(throttleIndicator1.getAircraftValue());
	}

	void ThrottleSection::onEventBeforeChildren(Event* event) {
		ToolbarSection::onEventBeforeChildren(event);
		
		if (!isFocused())
			return;
		
		if (event->getTypeID() == PointerDownEvent::typeID) {
			_pointerX = reinterpret_cast<PointerDownEvent*>(event)->getPosition().getX();
			
			setCaptured(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			if (_pointerX >= 0) {
				const auto dragX = reinterpret_cast<PointerDragEvent*>(event)->getPosition().getX();
				const auto deltaX = dragX - _pointerX;
				
				if (std::abs(deltaX) >= 5) {
					_pointerX = dragX;
					
					onAnyRotate((std::abs(deltaX) >= 10 ? 15 : 5) * (deltaX >= 0 ? 1 : -1));
				}
			}
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			_pointerX = -1;
			
			setCaptured(false);
		}
		if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
			onAnyRotate(reinterpret_cast<EncoderValueChangedEvent*>(event)->getDPSFactor(60, 10, 1));
			
			event->setHandled(true);
		}
	}
	
	void ThrottleSection::onAnyRotate(int32_t change) {
		auto& rc = RC::getInstance();
		
		rc.getRemoteData().throttle_0_255 = addSaturating(rc.getRemoteData().throttle_0_255, change * 0xFF / 100);
		rc.getAudioPlayer().playFeedback();
		
		invalidate();
	}
}