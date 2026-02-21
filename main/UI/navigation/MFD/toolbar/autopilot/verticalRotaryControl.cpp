#include "verticalRotaryControl.h"

#include "rc.h"

namespace pizda {
	VerticalRotaryControlStab::VerticalRotaryControlStab() {
		setDigitCount(3);
		setSignVisible(true);

		setActiveColor(&Theme::yellow);
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

		setValue(Units::convertDistance(RC::getInstance().getAircraftData().raw.autopilot.targetAltitudeM, DistanceUnit::meter, DistanceUnit::foot));
	}
	
	VerticalRotaryControl::VerticalRotaryControl() {
		setVariants({
			&seven,
			&stab,
			&ALT
		});

		seven.setValue(RC::getInstance().getSettings().autopilot.altitudeFt);

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
	
	void VerticalRotaryControl::onPress() {
		RotaryControl::onPress();

		auto& rc = RC::getInstance();

		switch (getVariantIndex()) {
			case 0: {
				rc.getRemoteData().autopilot.verticalMode =
					rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::flc
					? AutopilotVerticalMode::dir
					: AutopilotVerticalMode::flc;

				rc.getTransceiver().enqueueAutopilot(RemoteAuxiliaryAutopilotPacketType::setAltitude);

				break;
			}
			case 1: {
				rc.getRemoteData().autopilot.verticalMode =
					rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::stab
					? AutopilotVerticalMode::dir
					: AutopilotVerticalMode::stab;

				break;
			}
			default: {
				rc.getRemoteData().autopilot.verticalMode =
					rc.getAircraftData().raw.autopilot.verticalMode == AutopilotVerticalMode::alt
					? AutopilotVerticalMode::dir
					: AutopilotVerticalMode::alt;

				break;
			}
		}

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