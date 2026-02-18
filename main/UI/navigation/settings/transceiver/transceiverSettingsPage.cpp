#include "transceiverSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"
#include "utilities/string.h"

namespace pizda {
	TransceiverSettingsPage::TransceiverSettingsPage() {
		auto& rc = RC::getInstance();

		// Page title
		title.setText(L"Transceiver");

		// RF frequency
		Theme::apply(&_RFFrequency);
		_RFFrequency.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_RFFrequency.setText(std::to_wstring(rc.getSettings().transceiver.communication.frequencyHz / 1'000'000));
		rows += &_RFFrequencyTitle;

		horizontalRows[0].setOrientation(Orientation::horizontal);
		horizontalRows[0].setGap(8);
		rows += &horizontalRows[0];

		// Bandwidth
		_bandwidth.setItems(_bandwidthItems);
		_bandwidth.setItemCount(_bandwidthItemsLength);
		_bandwidth.setSelectedIndex(static_cast<uint8_t>(rc.getSettings().transceiver.communication.bandwidth));
		_bandwidth.setDialogTitle(L"Bandwidth");
		horizontalRows[0] += &_bandwidthTitle;

		// Coding rate
		_codingRate.setItems(_codingRateItems);
		_codingRate.setItemCount(_codingRateItemsLength);
		_codingRate.setSelectedIndex(static_cast<uint8_t>(rc.getSettings().transceiver.communication.codingRate));
		_codingRate.setDialogTitle(L"Coding rate");
		horizontalRows[0] += &_codingRateTitle;

		horizontalRows[1].setOrientation(Orientation::horizontal);
		horizontalRows[1].setGap(8);
		rows += &horizontalRows[1];

		// Spreading factor
		Theme::apply(&_spreadingFactor);
		_spreadingFactor.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_spreadingFactor.setText(std::to_wstring(rc.getSettings().transceiver.communication.spreadingFactor));
		horizontalRows[1] += &_spreadingFactorTitle;

		// Sync word
		Theme::apply(&_syncWord);
		_syncWord.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_syncWord.setText(std::to_wstring(rc.getSettings().transceiver.communication.syncWord));
		horizontalRows[1] += &_syncWordTitle;

		horizontalRows[2].setOrientation(Orientation::horizontal);
		horizontalRows[2].setGap(8);
		rows += &horizontalRows[2];

		// Preamble length
		Theme::apply(&_preambleLength);
		_preambleLength.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_preambleLength.setText(std::to_wstring(rc.getSettings().transceiver.communication.preambleLength));
		horizontalRows[2] += &_preambleLengthTitle;

		// Power
		Theme::apply(&_power);
		_power.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_power.setText(std::to_wstring(rc.getSettings().transceiver.communication.powerDBm));
		horizontalRows[2] += &_powerTitle;

		// Confirm button
		Theme::applyPrimary(&_confirmButton);
		_confirmButton.setText(L"Confirm");

		_confirmButton.setOnClick([this, &rc] {
			auto& settings = rc.getRemoteData().transceiver.communicationSettings;
			settings.frequencyHz = StringUtils::tryParseInt32Or(_RFFrequency.getText(), 0) * 1'000'000;
			settings.bandwidth = static_cast<SX1262::LoRaBandwidth>(_bandwidth.getSelectedIndex());
			settings.codingRate = static_cast<SX1262::LoRaCodingRate>(_codingRate.getSelectedIndex());
			settings.spreadingFactor = StringUtils::tryParseInt32Or(_spreadingFactor.getText(), 0);
			settings.syncWord = StringUtils::tryParseInt32Or(_syncWord.getText(), 0);
			settings.powerDBm = StringUtils::tryParseInt32Or(_power.getText(), 0);
			settings.preambleLength = StringUtils::tryParseInt32Or(_preambleLength.getText(), 0);
			settings.sanitize();

			rc.getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::XCVR);
		});

		rows += &_confirmButton;
	}
}
