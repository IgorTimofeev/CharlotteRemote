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
			~AutopilotSettingsPage() override;

		private:
			static int32_t _scrollPosition;

			// ----------------------------- Lateral -----------------------------

			TextField _latMaxRoll {};
			Titler _latMaxRollTitle { "Max roll ANG", &_latMaxRoll };

			RelativeStackLayout _latRow0 {};

			TextField _latTALPFFPS {};
			Titler _latTALPFFPSTitle { "TGT ANG LPFF", &_latTALPFFPS };

			TextField _latSMTAIRPS {};
			Titler _latSMTAIFRSTitle { "TGT ANG INC deg", &_latSMTAIRPS };

			TextField _latMaxAileronsFactor {};
			Titler _latMaxAileronsFactorTitle { "Max ailerons %", &_latMaxAileronsFactor };

			PIDReferencer _latYawToRollPID { "Yaw to roll" };
			Titler _latYawToRollPIDTitle { "Yaw to roll", &_latYawToRollPID };

			PIDReferencer _latRollToAileronsPID { "Roll to ailerons" };
			Titler _latRollToAileronsPIDTitle { "Roll to ailerons", &_latRollToAileronsPID };

			// ----------------------------- Vertical -----------------------------

			HorizontalSeparator _verSeparator {};
			TextView _verTitle { "Vertical"};

			TextField _verMaxPitch {};
			Titler _verMaxPitchTitle { "Max pitch ANG", &_verMaxPitch };

			RelativeStackLayout _verRow0 {};

			TextField _verTALPFFPS {};
			Titler _verTALPFFPSTitle { "TGT ANG LPFF", &_verTALPFFPS };

			TextField _verSMTARFPS {};
			Titler _verSMTAIRPSTitle { "TGT ANG INC deg", &_verSMTARFPS };

			TextField _verMaxElevatorFactor {};
			Titler _verMaxElevatorFactorTitle { "Max elevator %", &_verMaxElevatorFactor };

			PIDReferencer _verSpeedToPitchPID { "Speed to pitch" };
			Titler _verSpeedToPitchPIDTitle { "Speed to pitch", &_verSpeedToPitchPID };

			PIDReferencer _verAltitudeToPitchPID { "Altitude to pitch" };
			Titler _varAltitudeToPitchPIDTitle { "Altitude to pitch", &_verAltitudeToPitchPID };

			PIDReferencer _verPitchToElevatorPID { "Pitch to elevator" };
			Titler _verPitchToElevatorPIDTitle { "Pitch to elevator", &_verPitchToElevatorPID };

			// ----------------------------- Longitudinal -----------------------------

			HorizontalSeparator _lonSeparator {};
			TextView _lonTitle { "Longitudinal"};

			RelativeStackLayout _lonRow0 {};

			TextField _lonThrottleMin {};
			Titler _lonThrottleMinTitle { "Throttle idle %", &_lonThrottleMin };

			TextField _lonThrottleMax {};
			Titler _lonThrottleMaxTitle { "Throttle climb %", &_lonThrottleMax };

			PIDReferencer _lonSpeedToThrottlePID { "Speed to throttle" };
			Titler _lonSpeedToThrottlePIDTitle { "Speed to throttle", &_lonSpeedToThrottlePID };

			static void setupAnyTextField(TextField& textField, const std::string_view& text, const std::function<void()>& onEnter);
			static void setupFloatTextField(TextField& textField, float* value, float fallbackValue, float min, float max, RemoteSystemPacketType  packetType);
			static void setupRadTextField(TextField& textField, float* angleRad, float fallbackAngleDeg, RemoteSystemPacketType packetType);
			static void setupUint8PercentTextField(TextField& textField, uint8_t* percent, float fallbackPercent, RemoteSystemPacketType packetType);

			void addRow(RelativeStackLayout& row);
			void addPID(Titler& titler, PIDReferencer& referencer, RemoteSystemPacketType packetType, PIDCoefficients* settingsCoefficients);
	};
}
