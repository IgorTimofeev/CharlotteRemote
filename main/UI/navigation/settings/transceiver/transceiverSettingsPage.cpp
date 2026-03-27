#include "transceiverSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"
#include "utilities/string.h"

namespace pizda {
	TransceiverSettingsPage::TransceiverSettingsPage() {
		auto& rc = RC::getInstance();

		// Page title
		title.setText(L"Transceiver");

		// ----------------------------- Row 0 -----------------------------

		setupRow(_horizontalRows[0]);

		// RF frequency
		Theme::apply(&_RFFrequency);
		_RFFrequency.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_RFFrequency.setText(std::to_wstring(rc.getSettings().transceiver.communication.frequencyHz / 1'000'000));
		_horizontalRows[0] += &_RFFrequencyTitle;

		// Bandwidth
		_bandwidth.setItems(_bandwidthItems);
		_bandwidth.setItemCount(_bandwidthItemsLength);
		_bandwidth.setSelectedIndex(static_cast<uint8_t>(rc.getSettings().transceiver.communication.bandwidth));
		_bandwidth.setDialogTitle(L"Bandwidth");
		_horizontalRows[0] += &_bandwidthTitle;

		// ----------------------------- Row 1 -----------------------------

		setupRow(_horizontalRows[1]);

		// Coding rate
		_codingRate.setItems(_codingRateItems);
		_codingRate.setItemCount(_codingRateItemsLength);
		_codingRate.setSelectedIndex(static_cast<uint8_t>(rc.getSettings().transceiver.communication.codingRate));
		_codingRate.setDialogTitle(L"Coding rate");
		_horizontalRows[1] += &_codingRateTitle;

		// Spreading factor
		Theme::apply(&_spreadingFactor);
		_spreadingFactor.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_spreadingFactor.setText(std::to_wstring(rc.getSettings().transceiver.communication.spreadingFactor));
		_horizontalRows[1] += &_spreadingFactorTitle;

		// ----------------------------- Row 2 -----------------------------

		setupRow(_horizontalRows[2]);

		// Sync word
		Theme::apply(&_syncWord);
		_syncWord.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_syncWord.setText(std::to_wstring(rc.getSettings().transceiver.communication.syncWord));
		_horizontalRows[2] += &_syncWordTitle;

		// Preamble length
		Theme::apply(&_preambleLength);
		_preambleLength.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_preambleLength.setText(std::to_wstring(rc.getSettings().transceiver.communication.preambleLength));
		_horizontalRows[2] += &_preambleLengthTitle;

		// ----------------------------- Row 3 -----------------------------

		setupRow(_horizontalRows[3]);

		// Current limit
		Theme::apply(&_currentLimit);
		_currentLimit.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_currentLimit.setText(std::to_wstring(rc.getSettings().transceiver.communication.currentLimitMA));
		_horizontalRows[3] += &_currentLimitTitle;

		// Power
		Theme::apply(&_power);
		_power.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);
		_power.setText(std::to_wstring(rc.getSettings().transceiver.communication.powerDBm));
		_horizontalRows[3] += &_powerTitle;

		// ----------------------------- LE RAZJEBAT' BUTTON -----------------------------

		Theme::applyPrimary(&_confirmButton);
		_confirmButton.setText(L"Confirm");

		_confirmButton.setOnClick([this, &rc] {
			auto& settings = rc.getRemoteData().transceiver.communicationSettings;
			settings.frequencyHz = StringUtils::tryParseInt32Or(_RFFrequency.getText(), 0) * 1'000'000;
			settings.bandwidth = static_cast<SX1262::LoRaBandwidth>(_bandwidth.getSelectedIndex());
			settings.codingRate = static_cast<SX1262::LoRaCodingRate>(_codingRate.getSelectedIndex());
			settings.spreadingFactor = StringUtils::tryParseInt32Or(_spreadingFactor.getText(), 0);
			settings.syncWord = StringUtils::tryParseInt32Or(_syncWord.getText(), 0);
			settings.preambleLength = StringUtils::tryParseInt32Or(_preambleLength.getText(), 0);

			settings.currentLimitMA = StringUtils::tryParseInt32Or(_currentLimit.getText(), 60);
			settings.powerDBm = StringUtils::tryParseInt32Or(_power.getText(), 22);

			settings.sanitize();

			rc.getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::XCVR);
		});

		rows += &_confirmButton;
	}

	void TransceiverSettingsPage::setupRow(RelativeStackLayout& row) {
		row.setOrientation(Orientation::horizontal);
		row.setGap(8);
		rows += &row;
	}
}
