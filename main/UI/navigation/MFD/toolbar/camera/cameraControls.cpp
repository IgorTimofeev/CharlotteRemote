#include "cameraControls.h"

#include "rc.h"

namespace pizda {
	CameraControl::CameraControl(
		const int8_t minimum,
		const int8_t maximum,
		const std::wstring_view title,
		int8_t* settingsValue
	):
		SevenRotaryControl(
			3,
			minimum,
			maximum,
			false,
			1,
			10
		),
		_title(title),
		_settingsValue(settingsValue)
	{
		setVariants({
			&seven
		});

		setVariantIndex(0);

		seven.setSignVisible(true);
		seven.setValue(*_settingsValue);
	}

	std::wstring_view CameraControl::variantIndexToTitle(uint8_t index) {
		return _title;
	}

	bool CameraControl::isVariantEditable(uint8_t index) {
		return true;
	}

	void CameraControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);

		*_settingsValue = seven.getValue();

		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::camera);
	}
}
