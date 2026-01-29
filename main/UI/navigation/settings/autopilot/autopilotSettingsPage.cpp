#include "autopilotSettingsPage.h"

#include <format>
#include <algorithm>
#include <optional>

#include "rc.h"
#include "UI/theme.h"
#include "utils/string.h"

namespace pizda {
	AutopilotSettingsPage::AutopilotSettingsPage() {
		// Page title
		title.setText(L"PID testing");

		// -------------------------------- Chart --------------------------------

		chartEditor.setHeight(190);
		rows += &chartEditor;

		// PID
		PIDRow.setOrientation(Orientation::horizontal);
		PIDRow.setSpacing(5);
		rows += &PIDRow;

		// P
		Theme::apply(&PTextField);
		PTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
		PTextField.setText(L"0.1");

		PTextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		PIDRow += &PTitle;

		// I
		Theme::apply(&ITextField);
		ITextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
		ITextField.setText(L"20");

		ITextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		PIDRow += &ITitle;

		// D
		Theme::apply(&DTextField);
		DTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
		DTextField.setText(L"0.005");

		DTextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		PIDRow += &DTitle;

		updateChart();
	}

	void AutopilotSettingsPage::updateChart() {
		float p, i, d;

		if (!StringUtils::tryParseFloat(PTextField.getText(), p))
			p = 0;

		if (!StringUtils::tryParseFloat(ITextField.getText(), i))
			i = 0;

		if (!StringUtils::tryParseFloat(DTextField.getText(), d))
			d = 0;

		chartEditor.setCoefficients({ p, i, d });
	}
}
