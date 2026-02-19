#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/PID/PIDReferencer.h"

namespace pizda {
	using namespace YOBA;

	class AutopilotSettingsPage : public ScrollViewPage {
		public:
			AutopilotSettingsPage();

		private:
			PIDReferencer _targetToRollReferencer { L"Target > roll" };
			Titler _targetToRollReferencerTitle { L"Target > roll", &_targetToRollReferencer };

			PIDReferencer _targetToPitchReferencer { L"Target > pitch" };
			Titler _targetToPitchReferencerTitle { L"Target > pitch", &_targetToPitchReferencer };

			PIDReferencer _rollToAileronsReferencer { L"Roll > ailerons" };
			Titler _rollToAileronsReferencerTitle { L"Roll > ailerons", &_rollToAileronsReferencer };

			PIDReferencer _pitchToElevatorReferencer { L"Pitch > elevator" };
			Titler _pitchToElevatorReferencerTitle { L"Pitch > elevator", &_pitchToElevatorReferencer };
	};
}
