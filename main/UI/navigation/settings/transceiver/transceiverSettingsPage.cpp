#include "transceiverSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	TransceiverSettingsPage::TransceiverSettingsPage() {
		auto& rc = RC::getInstance();

		// Page title
		title.setText(L"Transceiver");

		// RF frequency
		_RFFrequency.setText(rc.getSettings().transceiver.spectrumScanning)
		rows += &_RFFrequencyTitle;
	}
}
