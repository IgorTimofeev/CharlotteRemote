#include "verticalRotaryControl.h"

#include "rc.h"

namespace pizda {
	VerticalRotaryControlStab::VerticalRotaryControlStab() {
		setDigitCount(3);
		setSignVisible(true);

		setActiveColor(&Theme::magenta1);
	}

	void VerticalRotaryControlStab::onTick() {
		RotaryControlSevenVariant::onTick();

		setValue(static_cast<int32_t>(std::round(toDegrees(RC::getInstance().getAircraftData().computed.autopilot.pitchRad))));
	}

	VerticalRotaryControlALT::VerticalRotaryControlALT() {
		setDigitCount(4);

		setActiveColor(&Theme::ocean);
	}

	void VerticalRotaryControlALT::onTick() {
		RotaryControlSevenVariant::onTick();

		auto& rc = RC::getInstance();

		setValue(
			rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::alt
			? Units::convertDistance(
				rc.getAircraftData().computed.coordinates.getAltitude(),
				DistanceUnit::meter,
				DistanceUnit::foot
			)
			: valueDashes
		);
	}
	
	VerticalRotaryControl::VerticalRotaryControl() {
		setVariants({
			&seven,
			&stab,
			&ALT
		});

		seven.setValue(RC::getInstance().getSettings().autopilot.altitudeFt);

		// Selecting variant
		switch (RC::getInstance().getSettings().autopilot.verticalMode) {
			case AutopilotVerticalMode::stab:
				setVariantIndex(1);
				break;

			case AutopilotVerticalMode::alt:
				setVariantIndex(2);
				break;

			default:
				setVariantIndex(0);
				break;
		}
	}
	
	std::wstring_view VerticalRotaryControl::variantIndexToTitle(const uint8_t index) {
		switch (index) {
			case 0: return L"FLC";
			case 1: return L"VSTB";
			default: return L"ALT";
		}
	}
	
	bool VerticalRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void VerticalRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);

		auto& rc = RC::getInstance();

		rc.getSettings().autopilot.altitudeFt = static_cast<uint16_t>(seven.getValue());
		rc.getSettings().autopilot.scheduleWrite();

		rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setAltitude);
	}

	void VerticalRotaryControl::onVariantChanged() {
		auto& rc = RC::getInstance();

		switch (getVariantIndex()) {
			case 0:
				rc.getSettings().autopilot.verticalMode = AutopilotVerticalMode::flc;
				break;

			case 1:
				rc.getSettings().autopilot.verticalMode = AutopilotVerticalMode::stab;
				break;

			default:
				rc.getSettings().autopilot.verticalMode = AutopilotVerticalMode::alt;
				break;
		}

		rc.getSettings().autopilot.scheduleWrite();
	}

	void VerticalRotaryControl::onPress() {
		RotaryControl::onPress();

		auto& rc = RC::getInstance();

		rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setVerticalMode);
	}
	
	void VerticalRotaryControl::onTick() {
		RotaryControl::onTick();
		
		switch (RC::getInstance().getAircraftData().raw.autopilot.verticalMode) {
			case AutopilotVerticalMode::dir:
				setBorderColor(nullptr);
				break;

			case AutopilotVerticalMode::stab:
				setBorderColor(getVariantIndex() == 1 ? &Theme::fg1 : &Theme::yellow);
				break;
				
			case AutopilotVerticalMode::flc:
				setBorderColor(getVariantIndex() == 0 ? &Theme::fg1 : &Theme::yellow);
				break;
				
			case AutopilotVerticalMode::alts:
				setBorderColor(getVariantIndex() == 0 ? &Theme::green1 : &Theme::yellow);
				break;
				
			case AutopilotVerticalMode::alt:
				setBorderColor(getVariantIndex() == 2 ? &Theme::ocean : &Theme::yellow);
				break;
		}
	}
}