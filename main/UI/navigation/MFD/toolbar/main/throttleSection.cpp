#include "throttleSection.h"

#include "rc.h"
#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "resources/sounds.h"

namespace pizda {
	ThrottleSection::ThrottleSection() : ToolbarSection(L"THR") {
		setFocusable(false);

		row.setGap(7);
		setDefaultMargin(row, 6);

		row += throttleIndicator1;
		row += throttleIndicator2;

		*this += row;
	}

	void ThrottleSection::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();

		// Throttles
		throttleIndicator1.setRemoteValue(rc.getAxes().getLeverLeft().getFilteredValueUint8());
		throttleIndicator1.setAircraftValue(static_cast<uint8_t>(rc.getAircraftData().computed.throttle_0_1 * 0xFF));
		
		throttleIndicator2.setRemoteValue(throttleIndicator1.getRemoteValue());
		throttleIndicator2.setAircraftValue(throttleIndicator1.getAircraftValue());
	}
}