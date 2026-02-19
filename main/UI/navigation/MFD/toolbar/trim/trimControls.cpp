#include "trimControls.h"

#include "rc.h"

namespace pizda {
	TrimControl::TrimControl(const std::wstring_view title, int8_t* settingsValue) : _title(title), _settingsValue(settingsValue) {
		setVariants({
			&seven
		});

		setVariantIndex(0);

		seven.setSignVisible(true);
		seven.setValue(*_settingsValue);
	}
	
	std::wstring_view TrimControl::variantIndexToTitle(uint8_t index) {
		return _title;
	}
	
	bool TrimControl::isVariantEditable(uint8_t index) {
		return true;
	}
	
	void TrimControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		*_settingsValue = seven.getValue();
		RC::getInstance().getSettings().controls.scheduleWrite();
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::trim);
	}
}