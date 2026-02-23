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
			Titler _RFFrequencyTitle { L"Freq [120; 960] (MHz)", &_RFFrequency };

			RelativeStackLayout horizontalRows[3] {};

			constexpr static uint8_t _bandwidthItemsLength = 10;
			std::wstring_view _bandwidthItems[_bandwidthItemsLength] {
				L"7.81",
				L"10.42",
				L"15.63",
				L"20.83",
				L"31.25",
				L"41.67",
				L"62.5",
				L"125.0",
				L"250.0",
				L"500.0"
			};

			ComboBox _bandwidth {};
			Titler _bandwidthTitle { L"BW (kHz)", &_bandwidth };

			constexpr static uint8_t _codingRateItemsLength = 4;
			std::wstring_view _codingRateItems[_codingRateItemsLength] {
				L"4/5",
				L"4/6",
				L"4/7",
				L"4/8"
			};

			ComboBox _codingRate {};
			Titler _codingRateTitle { L"CR", &_codingRate };

			TextField _spreadingFactor {};
			Titler _spreadingFactorTitle { L"SF [5; 12]", &_spreadingFactor };

			TextField _syncWord {};
			Titler _syncWordTitle { L"SW [0; 255]", &_syncWord };

			TextField _preambleLength {};
			Titler _preambleLengthTitle { L"PL [0; 65535] s", &_preambleLength };

			TextField _power {};
			Titler _powerTitle { L"PWR [-17; 22] (dBm)", &_power };

			Button _confirmButton {};
	};
}
