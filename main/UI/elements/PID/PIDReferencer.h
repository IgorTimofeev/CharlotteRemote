#pragma once

#include <format>
#include <functional>

#include <YOBA/UI.h>

#include "UI/elements/referencer.h"
#include "UI/elements/PID/PIDChart.h"
#include "UI/elements/dialogs/scrollViewDialog.h"
#include "UI/theme.h"

#include "types/generic.h"
#include "utilities/string.h"

namespace pizda {
	using namespace YOBA;

	class PIDReferencerDialog : public ScrollViewDialog {
		public:
			PIDReferencerDialog(const std::wstring_view titleText, const PIDCoefficients& coefficients, const std::function<void(const PIDCoefficients&)>& onConfirm) {
				title.setText(titleText);

				// Chart
				chart.setHeight(120);
				chart.setDeltaTime(1.f / 20.f);
				chart.setStepsQuantity(10);
				chart.setBackgroundColor(&Theme::bg3);
				rows += &chart;

				// PID
				PIDRow.setOrientation(Orientation::horizontal);
				PIDRow.setSpacing(5);
				rows += &PIDRow;

				// P
				Theme::apply(&PTextField);
				PTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
				PTextField.setText(StringUtils::toWString(coefficients.p));

				PTextField.setOnInput([this](Key, std::optional<std::wstring_view>) {
					updateChart();
				});

				PIDRow += &PTitle;

				// I
				Theme::apply(&ITextField);
				ITextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
				ITextField.setText(StringUtils::toWString(coefficients.i));

				ITextField.setOnInput([this](Key, std::optional<std::wstring_view>) {
					updateChart();
				});

				PIDRow += &ITitle;

				// D
				Theme::apply(&DTextField);
				DTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric | KeyboardLayoutOptions::allowDecimal);
				DTextField.setText(StringUtils::toWString(coefficients.d));

				DTextField.setOnInput([this](Key, std::optional<std::wstring_view>) {
					updateChart();
				});

				PIDRow += &DTitle;

				// Confirm button
				Theme::applyPrimary(&_confirmButton);
				_confirmButton.setText(L"Confirm");

				_confirmButton.setOnClick([this, onConfirm] {
					Application::getCurrent()->scheduleOnTick([this, onConfirm] {
						onConfirm(chart.getCoefficients());

						hide();
						delete this;
					});
				});

				rows += &_confirmButton;

				// Initialization
				updateChart();
			}

		private:

			PIDChart chart {};

			RelativeStackLayout PIDRow {};

			TextField PTextField {};
			Titler PTitle { L"P", &PTextField };

			TextField ITextField {};
			Titler ITitle { L"I", &ITextField };

			TextField DTextField {};
			Titler DTitle { L"D", &DTextField };

			Button _confirmButton {};

			void updateChart() {
				float p, i, d;

				if (!StringUtils::tryParseFloat(PTextField.getText(), p))
					p = 0;

				if (!StringUtils::tryParseFloat(ITextField.getText(), i))
					i = 0;

				if (!StringUtils::tryParseFloat(DTextField.getText(), d))
					d = 0;

				chart.setCoefficients({ p, i, d });
			}
	};

	class PIDReferencer : public Referencer {
		public:
			PIDReferencer(const std::wstring_view dialogTitle) {
				_textsRow.setOrientation(Orientation::horizontal);
				_textsRow.setSpacing(5);
				setDefaultMargin(&_textsRow, Margin(10));
				*this += &_textsRow;

				addCoeffText(_textP, &Theme::yellow);
				addSuffixText(_textP_I, L"P +");
				addCoeffText(_textI, &Theme::magenta1);
				addSuffixText(_textI_D, L"I +");
				addCoeffText(_textD, &Theme::ocean);
				addSuffixText(_textD_, L"D");

				setOnClick([this, dialogTitle] {
					(
						new PIDReferencerDialog(
							dialogTitle,
							_coefficients,
							[this](const PIDCoefficients& coefficients) {
								setCoefficients(coefficients);
							}
						)
					)->show();
				});
			}

			void setCoefficients(const PIDCoefficients& coefficients) {
				_coefficients = coefficients;

				_textP.setText(StringUtils::toWString(_coefficients.p));
				_textI.setText(StringUtils::toWString(_coefficients.i));
				_textD.setText(StringUtils::toWString(_coefficients.d));
			}

		private:
			PIDCoefficients _coefficients {};

			StackLayout _rows {};

			StackLayout _textsRow {};
			TextView _textP {};
			TextView _textP_I {};
			TextView _textI {};
			TextView _textI_D {};
			TextView _textD {};
			TextView _textD_ {};

			void addText(TextView& text, const Color* color) {
				text.setTextColor(color);
				text.setVerticalAlignment(Alignment::end);
				_textsRow += &text;
			}

			void addCoeffText(TextView& text, const Color* color) {
				text.setFont(&Theme::fontSmall);
				text.setFontScale(2);

				addText(text, color);
			}

			void addSuffixText(TextView& text, const std::wstring_view suffix) {
				text.setMargin(Margin(0, 0, 0, 1));
				text.setFont(&Theme::fontNormal);
				text.setText(suffix);

				addText(text, &Theme::fg4);
			}
	};
}