#include "motorsSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	MotorEditor::MotorEditor(std::wstring_view title, SettingsMotor* settings) : Titler(title), _settings(settings) {
		_row.setOrientation(Orientation::horizontal);
		_row.setSpacing(5);
		
		*this += &_row;
		
		// Min
		Theme::apply(&_min);
		_min.setText(std::to_wstring(_settings->min));
		_row += &_min;
		
		// Max
		Theme::apply(&_max);
		_max.setText(std::to_wstring(_settings->max));
		_row += &_max;
		
		// Startup
		Theme::apply(&_startup);
		_startup.setText(std::to_wstring(_settings->startup));
		_row += &_startup;
		
		// Offset
		Theme::apply(&_offset);
		_offset.setText(std::to_wstring(_settings->offset));
		_row += &_offset;
		
		// Reverse
		Theme::applySecondary(&_reverse);
		_reverse.setWidth(20);
		_reverse.setText(L"<->");
		_reverse.setToggle(true);
		_reverse.setActive(_settings->reverse);
		
		_row.setAutoSize(&_reverse);
		_row += &_reverse;
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