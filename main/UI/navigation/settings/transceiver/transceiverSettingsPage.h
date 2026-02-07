#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/PID/PIDReferencer.h"

namespace pizda {
	using namespace YOBA;

	class TransceiverSettingsPage : public ScrollViewPage {
		public:
			TransceiverSettingsPage();

		private:
			TextField _RFFrequency {};
			Titler _RFFrequencyTitle { L"RF frequency (Hz)", &_RFFrequency };

	};
}
