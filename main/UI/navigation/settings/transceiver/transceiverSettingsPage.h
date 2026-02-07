#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/comboBox.h"
#include "UI/elements/PID/PIDReferencer.h"

namespace pizda {
	using namespace YOBA;

	class TransceiverSettingsPage : public ScrollViewPage {
		public:
			TransceiverSettingsPage();

		private:
			TextField _RFFrequency {};
			Titler _RFFrequencyTitle { L"RF frequency (MHz)", &_RFFrequency };

			ComboBox _bandwidth {};

			constexpr static uint8_t _bandwidthItemsLength = 8;
			std::wstring_view _bandwidthItems[_bandwidthItemsLength] {
				L"7.8 kHz",
				L"10.4 kHz",
				L"15.6 kHz",
				L"20.8 kHz",
				L"31.25 kHz",
				L"41.7 kHz",
				L"62.5 kHz",
				L"125.0 kHz"
			};

			Titler _bandwidthTitle { L"Bandwidth", &_bandwidth };

			ComboBox _codingRate {};

			constexpr static uint8_t _codingRateItemsLength = 4;
			std::wstring_view _codingRateItems[_codingRateItemsLength] {
				L"4/5",
				L"4/6",
				L"4/7",
				L"4/8"
			};

			Titler _codingRateTitle { L"Coding rate", &_codingRate };

			TextField _spreadingFactor {};
			Titler _spreadingFactorTitle { L"Spreading factor [5; 12]", &_spreadingFactor };

			TextField _syncWord {};
			Titler _syncWordTitle { L"Sync word [0; 255]", &_syncWord };

			TextField _preambleLength {};
			Titler _preambleLengthTitle { L"Preamble length [0; 65535]", &_preambleLength };

			TextField _power {};
			Titler _powerTitle { L"Power ([-17; 22] dBm", &_power };

			Button _confirmButton {};
	};
}
