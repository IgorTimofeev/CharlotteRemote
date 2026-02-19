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

		setValue(RC::getInstance().getAircraftData().raw.autopilot.targetAltitudeM);
	}
	
	VerticalRotaryControl::VerticalRotaryControl() {
		setVariants({
			&seven,
			&stab,
			&ALT
		});

		switch (RC::getInstance().getRemoteData().autopilot.verticalMode) {
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

		seven.setValue(RC::getInstance().getSettings().autopilot.altitudeFt);
	}
	
	std::wstring_view VerticalRotaryControl::variantIndexToTitle(const uint8_t index) {
		switch (index) {
			case 0: return L"FLC";
			case 1: return L"STB";
			default: return L"ALT";
		}
	}
	
	bool VerticalRotaryControl::isVariantEditable(const uint8_t index) {
		return index == 0;
	}
	
	void VerticalRotaryControl::onRotate(const bool clockwise, const bool big) {
		SevenRotaryControl::onRotate(clockwise, big);
		
		RC::getInstance().getSettings().autopilot.altitudeFt = static_cast<uint16_t>(seven.getValue());
		RC::getInstance().getSettings().autopilot.scheduleWrite();
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
	}
	
	void VerticalRotaryControl::onPress() {
		RotaryControl::onPress();
		
		AutopilotVerticalMode newMode;
		
		switch (getVariantIndex()) {
			case 0: {
				newMode = AutopilotVerticalMode::flc;
				break;
			}
			case 1: {
				newMode = AutopilotVerticalMode::stab;
				RC::getInstance().getRemoteData().autopilot.altitudeHoldFt = RC::getInstance().getAircraftData().computed.altitudeFt;
				break;
			}
			default: {
				newMode = AutopilotVerticalMode::alt;
				RC::getInstance().getRemoteData().autopilot.altitudeHoldFt = RC::getInstance().getAircraftData().computed.altitudeFt;
				break;
			}
		}
		
		RC::getInstance().getRemoteData().autopilot.verticalMode =
			newMode == RC::getInstance().getRemoteData().autopilot.verticalMode
			? AutopilotVerticalMode::dir
			: newMode;
		
		RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::autopilot);
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
				setBorderColor(getVariantIndex() == 0 ? &Theme::fg1 : &Theme::yellow);
				break;
				
			case AutopilotVerticalMode::alt:
				setBorderColor(getVariantIndex() == 2 ? &Theme::fg1 : &Theme::yellow);
				break;
		}
	}
}