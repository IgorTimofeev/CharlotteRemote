#include "autopilotSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"
#include "types/generic.h"

namespace pizda {
	AutopilotSettingsPage::AutopilotSettingsPage() {
		// Page title
		title.setText(L"PID");

		const auto addPizda = [this](Titler& titler, PIDReferencer& referencer, AutopilotPIDType PIDType, PIDCoefficients* settingsCoefficients) {
			referencer.setCoefficients(*settingsCoefficients);

			referencer.setOnCoefficientsChanged([settingsCoefficients, PIDType](const PIDCoefficients& newCoefficients) {
				auto& rc = RC::getInstance();

				*settingsCoefficients = newCoefficients;
				rc.getSettings().PID.scheduleWrite();

				rc.getRemoteData().autopilot.PID.type = PIDType;
				rc.getRemoteData().autopilot.PID.coefficients = newCoefficients;

				rc.getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::PID);
			});

			rows += &titler;
		};

		auto& rc = RC::getInstance();

		addPizda(
			_targetToRollReferencerTitle,
			_targetToRollReferencer,
			AutopilotPIDType::targetToRoll,
			&rc.getSettings().PID.targetToRoll
		);

		addPizda(
			_targetToPitchReferencerTitle,
			_targetToPitchReferencer,
			AutopilotPIDType::targetToPitch,
			&rc.getSettings().PID.targetToPitch
		);

		addPizda(
			_rollToAileronsReferencerTitle,
			_rollToAileronsReferencer,
			AutopilotPIDType::rollToAilerons,
			&rc.getSettings().PID.rollToAilerons
		);

		addPizda(
			_pitchToElevatorReferencerTitle,
			_pitchToElevatorReferencer,
			AutopilotPIDType::pitchToElevator,
			&rc.getSettings().PID.pitchToElevator
		);
	}
}
