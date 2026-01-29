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

		chart.setHeight(160);
		rows += &chart;

		// PID
		PIDRow.setOrientation(Orientation::horizontal);
		PIDRow.setSpacing(5);
		rows += &PIDRow;

		// P
		Theme::apply(&PTextField);
		PTextField.setText(L"0.1");

		PTextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		PIDRow += &PTitle;

		// I
		Theme::apply(&ITextField);
		ITextField.setText(L"20");

		ITextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		PIDRow += &ITitle;

		// D
		Theme::apply(&DTextField);
		DTextField.setText(L"0.005");

		DTextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		PIDRow += &DTitle;

		// Step & interval
		stepAndIntervalRow.setOrientation(Orientation::horizontal);
		stepAndIntervalRow.setSpacing(5);
		rows += &stepAndIntervalRow;

		// Step
		Theme::apply(&stepTextField);
		stepTextField.setText(L"10");

		stepTextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		stepAndIntervalRow += &stepTitle;

		// Delta time
		Theme::apply(&deltaTimeTextField);
		deltaTimeTextField.setText(L"0.05");

		deltaTimeTextField.input += [this](Key, std::optional<std::wstring_view>) {
			updateChart();
		};

		stepAndIntervalRow += &intervalTitle;

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

		chart.setPIDCoefficients({ p, i, d });

		int32_t step;
		if (!StringUtils::tryParseInt32(stepTextField.getText(), step))
			step = 1;

		chart.setStep(std::clamp<int32_t>(step, 1, 100));

		float interval;
		if (!StringUtils::tryParseFloat(deltaTimeTextField.getText(), interval))
			interval = 1;

		chart.setDeltaTime(std::clamp<float>(interval, 0.0001, 100));
	}
}
