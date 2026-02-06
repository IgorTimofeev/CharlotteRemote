#include "autopilotSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	AutopilotSettingsPage::AutopilotSettingsPage() {
		// Page title
		title.setText(L"PID tuning");

		rows += &_autothrottleReferencerTitle;
		rows += &_rollReferencerTitle;
		rows += &_aileronsReferencerTitle;
		rows += &_pitchReferencerTitle;

		_autothrottleReferencer.setCoefficients({0.1f, 20, 0.005f });
		_rollReferencer.setCoefficients({0.1f, 20, 0.005f });
		_aileronsReferencer.setCoefficients({0.1f, 20, 0.005f });
		_pitchReferencer.setCoefficients({0.1f, 20, 0.005f });
	}
}
