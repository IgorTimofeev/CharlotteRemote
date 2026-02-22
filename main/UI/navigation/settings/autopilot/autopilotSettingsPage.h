#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "systems/transceiver/packet.h"
#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/PID/PIDReferencer.h"
#include "UI/elements/separator.h"

namespace pizda {
	using namespace YOBA;

	class AutopilotSettingsPage : public ScrollViewPage {
		public:
			AutopilotSettingsPage();

		private:
			// ----------------------------- Lateral -----------------------------

			TextField _latMaxRoll {};
			Titler _latMaxRollTitle { L"Max roll ang", _latMaxRoll };

			RelativeStackLayout _latRow0 {};

			TextField _latTALPFFPS {};
			Titler _latTALPFFPSTitle { L"Ang LPFF", _latTALPFFPS };

			TextField _latSMTAIFPS {};
			Titler _latSMTAIFPSTitle { L"Stab ang inc F", _latSMTAIFPS };

			TextField _latMaxAileronsFactor {};
			Titler _latMaxAileronsFactorTitle { L"Max ailerons F", _latMaxAileronsFactor };

			PIDReferencer _latYawToRollPID { L"Yaw to roll" };
			Titler _latYawToRollPIDTitle { L"Yaw to roll", _latYawToRollPID };

			PIDReferencer _latRollToAileronsPID { L"Roll to ailerons" };
			Titler _latRollToAileronsPIDTitle { L"Roll to ailerons", _latRollToAileronsPID };

			// ----------------------------- Vertical -----------------------------

			HorizontalSeparator _verSeparator {};
			TextView _verTitle { L"Vertical"};

			TextField _verMaxPitch {};
			Titler _verMaxPitchTitle { L"Max pitch ang", _verMaxPitch };

			RelativeStackLayout _verRow0 {};

			TextField _verTALPFFPS {};
			Titler _verTALPFFPSTitle { L"Ang LPFF", _verTALPFFPS };

			TextField _verSMTAIFPS {};
			Titler _verSMTAIFPSTitle { L"Stab ang inc F", _verSMTAIFPS };

			TextField _verMaxElevatorFactor {};
			Titler _verMaxElevatorFactorTitle { L"Max elevator F", _verMaxElevatorFactor };

			PIDReferencer _verSpeedToPitchPID { L"Speed to pitch" };
			Titler _verSpeedToPitchPIDTitle { L"Speed to pitch", _verSpeedToPitchPID };

			PIDReferencer _verAltitudeToPitchPID { L"Altitude to pitch" };
			Titler _varAltitudeToPitchPIDTitle { L"Altitude to pitch", _verAltitudeToPitchPID };

			PIDReferencer _verPitchToElevatorPID { L"Pitch to elevator" };
			Titler _verPitchToElevatorPIDTitle { L"Pitch to elevator", _verPitchToElevatorPID };

			// ----------------------------- Longitudinal -----------------------------

			HorizontalSeparator _lonSeparator {};
			TextView _lonTitle { L"Longitudinal"};

			TextField _lonThrottleLPFFPS {};
			Titler _lonThrottleLPFFPSTitle { L"Throttle LPFF", _lonThrottleLPFFPS };

			PIDReferencer _lonSpeedToThrottlePID { L"Speed to throttle" };
			Titler _lonSpeedToThrottlePIDTitle { L"Speed to throttle", _lonSpeedToThrottlePID };

			void setupAnyTextField(TextField& textField, const std::wstring_view& text, const std::function<void()>& onEnter);
			void setupFloatTextField(TextField& textField, float* value, float fallbackValue, float min, float max, RemoteAuxiliaryAutopilotPacketType  packetType);
			void setupRadTextField(TextField& textField, float* angleRad, float fallbackAngleDeg, RemoteAuxiliaryAutopilotPacketType packetType);
			void addRow(RelativeStackLayout& row);
			void addPID(Titler& titler, PIDReferencer& referencer, RemoteAuxiliaryAutopilotPacketType packetType, PIDCoefficients* settingsCoefficients);
	};
}
