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
			// Max angles
			RelativeStackLayout _anglesRow {};

			TextField _anglesMaxRoll {};
			Titler _anglesMaxRollTitle { L"Max roll", &_anglesMaxRoll };

			TextField _anglesMaxPitch {};
			Titler _anglesMaxPitchTitle { L"Max pitch", &_anglesMaxPitch };

			// LPFFs
			HorizontalSeparator _targetAnglesSeparator {};
			TextView _targetAnglesTitle { L"Target angle"};

			RelativeStackLayout _targetAnglesRow {};

			TextField _targetAngleLPFF {};
			Titler _targetAngleLPFFTitle { L"LPFF", &_targetAngleLPFF };

			TextField _stabAngleIncrement {};
			Titler _stabAngleIncrementTitle { L"Stab TA incr", &_stabAngleIncrement };

			// Surfaces
			HorizontalSeparator _surfacesSeparator {};
			TextView _surfacesTitle { L"Surfaces"};

			RelativeStackLayout _surfacesRow {};

			TextField _maxAileronsFactor {};
			Titler _maxAileronsFactorTitle { L"Max ailerons", &_maxAileronsFactor };

			TextField _maxElevatorFactor {};
			Titler _maxElevatorFactorTitle { L"Max elevator", &_maxElevatorFactor };

			// PIDs
			HorizontalSeparator _PIDSeparator {};
			TextView _PIDTitle { L"PID tuning"};

			PIDReferencer _yawToRollPID { L"Yaw to roll" };
			Titler _yawToRollPIDTitle { L"Yaw to roll", &_yawToRollPID };

			PIDReferencer _altitudeToPitchPID { L"Altitude to pitch" };
			Titler _altitudeToPitchPIDTitle { L"Altitude to pitch", &_altitudeToPitchPID };

			PIDReferencer _speedToPitchPID { L"Speed to pitch" };
			Titler _speedToPitchPIDTitle { L"Speed to pitch", &_speedToPitchPID };

			PIDReferencer _rollToAileronsPID { L"Roll to ailerons" };
			Titler _rollToAileronsPIDTitle { L"Roll to ailerons", &_rollToAileronsPID };

			PIDReferencer _pitchToElevatorPID { L"Pitch to elevator" };
			Titler _pitchToElevatorPIDTitle { L"Pitch to elevator", &_pitchToElevatorPID };

			void setupAnyTextField(TextField& textField, const std::wstring_view& text, const std::function<void()>& onEnter);
			void setupFloatTextField(TextField& textField, float* value, float fallbackValue, float min, float max, RemoteAuxiliaryAutopilotPacketType
			                         packetType);
			void setupRadTextField(TextField& textField, float* angleRad, float fallbackAngleDeg, RemoteAuxiliaryAutopilotPacketType packetType);
	};
}
