#pragma once

#include <functional>

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
			PIDReferencer _autothrottleReferencer { L"Autothrottle" };
			Titler _autothrottleReferencerTitle { L"Autothrottle", &_autothrottleReferencer };

			PIDReferencer _rollReferencer { L"Roll" };
			Titler _rollReferencerTitle { L"Roll", &_rollReferencer };

			PIDReferencer _aileronsReferencer { L"Ailerons" };
			Titler _aileronsReferencerTitle { L"Ailerons", &_aileronsReferencer };

			PIDReferencer _pitchReferencer { L"Pitch" };
			Titler _pitchReferencerTitle { L"Pitch", &_pitchReferencer };

	};
}
