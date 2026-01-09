#include "trimControls.h"

#include "rc.h"

namespace pizda {
	TrimControl::TrimControl(std::wstring_view title, int8_t* settingsValue) : _title(title), _settingsValue(settingsValue) {
		seven.setSignVisible(true);
		seven.setValue(*_settingsValue);
		
		setVariants({
			&seven
		});
	}
	
	std::wstring_view TrimControl::variantIndexToTitle(uint8_t index) {
		return _title;
	}
	
	bool TrimControl::isVariantEditable(uint8_t index) {
		return true;
	}
	
	void TrimControl::onRotate(bool clockwise, bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		*_settingsValue = seven.getValue();
		RC::getInstance().getSettings().controls.scheduleWrite();
		
		RC::getInstance().getPacketHandler().enqueue(RemotePacketType::trim);
	}
}