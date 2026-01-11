#include "motorsSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	MotorEditor::MotorEditor(std::wstring_view title, SettingsMotor* settings) : Titler(title), _settings(settings) {
		*this += &_mainLayout;
		
		// Reverse
		Theme::applySecondary(&_reverse);
		_reverse.setHeight(14);
		_reverse.setVerticalAlignment(Alignment::end);
		
		_reverse.setDefaultBackgroundColor(&Theme::bg3);
		_reverse.setDefaultTextColor(&Theme::fg2);
		
		_reverse.setActiveBackgroundColor(&Theme::fg1);
		_reverse.setActiveTextColor(&Theme::bg1);
		
		_reverse.setContentMargin(Margin(0, Theme::cornerRadius / 2, 0, 0));
		_reverse.setText(L"Reverse");
		_reverse.setToggle(true);
		_reverse.setActive(_settings->reverse);
		
		_mainLayout += &_reverse;
		
		// Background rect
		_rowsBackgroundRect.setMargin(Margin(0, 0, 0, _reverse.getSize().getHeight() - Theme::cornerRadius - 1));
		_rowsBackgroundRect.setFillColor(&Theme::bg2);
		_rowsBackgroundRect.setBorderColor(&Theme::bg3);
		_rowsBackgroundRect.setCornerRadius(Theme::cornerRadius);
		_mainLayout += &_rowsBackgroundRect;
		
		// Rows
		_rows.setSpacing(5);
		_rows.setMargin(Margin(10, 10, 8, _rowsBackgroundRect.getMargin().getBottom() + 10));
		_mainLayout += &_rows;
		
		// -------------------------------- row0 --------------------------------
		
		_row0.setOrientation(Orientation::horizontal);
		_row0.setSpacing(Theme::spacing);
		_rows += &_row0;
		
		// Min
		Theme::apply(&_min);
		_min.setText(std::to_wstring(_settings->min));
		_row0 += &_minTitle;
		
		// Max
		Theme::apply(&_max);
		_max.setText(std::to_wstring(_settings->max));
		_row0 += &_maxTitle;
		
		
		_row1.setOrientation(Orientation::horizontal);
		_row1.setSpacing(Theme::spacing);
		_rows += &_row1;
		
		// -------------------------------- row1 --------------------------------
		
		// Startup
		Theme::apply(&_startup);
		_startup.setText(std::to_wstring(_settings->startup));
		_row1 += &_startupTitle;
		
		// Offset
		Theme::apply(&_offset);
		_offset.setText(std::to_wstring(_settings->offset));
		_row1 += &_offsetTitle;
	}
	
	void MotorEditor::toSettings() {
		int32_t result = 0;
		
		// Min
		if (!StringUtils::tryParseInt32(_min.getText(), result))
			result = 1000;
		
		_settings->min = static_cast<uint16_t>(result);
		
		// Max
		if (!StringUtils::tryParseInt32(_max.getText(), result))
			result = 2000;
		
		_settings->max = static_cast<uint16_t>(result);
		
		// Startup
		if (!StringUtils::tryParseInt32(_startup.getText(), result))
			result = 1500;
		
		_settings->startup = static_cast<uint16_t>(result);
		
		// Offset
		if (!StringUtils::tryParseInt32(_offset.getText(), result))
			result = 0;
		
		_settings->offset = static_cast<int16_t>(result);
		
		// Reverse
		_settings->reverse = _reverse.isActive();
		
		_settings->sanitize();
	}
	
	MotorsSettingsPage::MotorsSettingsPage() :
		throttle(L"Throttle", &RC::getInstance().getSettings().motors.throttle),
		noseWheel(L"Nose wheel", &RC::getInstance().getSettings().motors.noseWheel),
		aileronLeft(L"Left aileron", &RC::getInstance().getSettings().motors.aileronLeft),
		aileronRight(L"Right aileron", &RC::getInstance().getSettings().motors.aileronRight),
		flapLeft(L"Left flap", &RC::getInstance().getSettings().motors.flapLeft),
		flapRight(L"Right flap", &RC::getInstance().getSettings().motors.flapRight),
		tailLeft(L"Left tail", &RC::getInstance().getSettings().motors.tailLeft),
		tailRight(L"Right tail", &RC::getInstance().getSettings().motors.tailRight)
	{
		// Page title
		title.setText(L"Motors");
		
		// Content
		rows += &throttle;
		rows += &noseWheel;
		rows += &aileronLeft;
		rows += &aileronRight;
		rows += &flapLeft;
		rows += &flapRight;
		rows += &tailLeft;
		rows += &tailRight;
		
		// Confirm
		Theme::applyPrimary(&confirmButton);
		confirmButton.setText(L"Confirm");
		
		confirmButton.click += [this]() {
			throttle.toSettings();
			noseWheel.toSettings();
			aileronLeft.toSettings();
			aileronRight.toSettings();
			flapLeft.toSettings();
			flapRight.toSettings();
			tailLeft.toSettings();
			tailRight.toSettings();
			
			RC::getInstance().getSettings().motors.scheduleWrite();
			
			// FUCKING SEND
		};
	}
}