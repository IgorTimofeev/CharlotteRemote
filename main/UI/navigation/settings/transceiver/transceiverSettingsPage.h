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
			~TransceiverSettingsPage() override;

		private:
			static int32_t _scrollPosition;

			TextField _RFFrequency {};
			Titler _RFFrequencyTitle { "RF [120; 960] (MHz)", &_RFFrequency };

			RelativeStackLayout _horizontalRows[4] {};

			constexpr static uint8_t _bandwidthItemsLength = 10;
			std::string_view _bandwidthItems[_bandwidthItemsLength] {
				"7.81",
				"10.42",
				"15.63",
				"20.83",
				"31.25",
				"41.67",
				"62.5",
				"125.0",
				"250.0",
				"500.0"
			};

			ComboBox _bandwidth {};
			Titler _bandwidthTitle { "BW (kHz)", &_bandwidth };

			constexpr static uint8_t _codingRateItemsLength = 4;
			std::string_view _codingRateItems[_codingRateItemsLength] {
				"4/5",
				"4/6",
				"4/7",
				"4/8"
			};

			ComboBox _codingRate {};
			Titler _codingRateTitle { "CR", &_codingRate };

			TextField _spreadingFactor {};
			Titler _spreadingFactorTitle { "SF [5; 12]", &_spreadingFactor };

			TextField _syncWord {};
			Titler _syncWordTitle { "SW [0; 255]", &_syncWord };

			TextField _preambleLength {};
			Titler _preambleLengthTitle { "PL [0; 65535] s", &_preambleLength };

			TextField _currentLimit {};
			Titler _currentLimitTitle { "OCP [0; 140] (mA)", &_currentLimit };

			TextField _power {};
			Titler _powerTitle { "PWR [-17; 22] (dBm)", &_power };

			Button _confirmButton {};

			void setupRow(RelativeStackLayout& row);
	};
}
