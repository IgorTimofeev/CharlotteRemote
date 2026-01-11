#include "motorsSettingsPage.h"

#include <optional>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	MotorEditor::MotorEditor(std::wstring_view title, MotorConfiguration* settings) : Titler(title), _settings(settings) {
		*this += &_mainLayout;
		
		// Reverse
		Theme::applySecondary(&_reverse);
		_reverse.setHeight(13);
		_reverse.setVerticalAlignment(Alignment::end);
		
		_reverse.setDefaultBackgroundColor(&Theme::bg3);
		_reverse.setDefaultTextColor(&Theme::fg2);
		
		_reverse.setActiveBackgroundColor(&Theme::fg1);
		_reverse.setActiveTextColor(&Theme::bg1);
		
		_reverse.setContentMargin(Margin(0, 2, 0, 0));
		_reverse.setFont(&Theme::fontSmall);
		_reverse.setText(L"Reverse");
		_reverse.setToggle(true);
		_reverse.setActive(_settings->reverse);
		
		_reverse.click += [this]() {
			changed();
		};
		
		_mainLayout += &_reverse;
		
		// Background rect
		_backgroundRect.setMargin(Margin(0, 0, 0, _reverse.getSize().getHeight() - Theme::cornerRadius - 1));
		_backgroundRect.setFillColor(&Theme::bg2);
		_backgroundRect.setBorderColor(&Theme::bg3);
		_backgroundRect.setCornerRadius(Theme::cornerRadius);
		_mainLayout += &_backgroundRect;
		
		// Row
		_row.setOrientation(Orientation::horizontal);
		_row.setSpacing(5);
		_row.setMargin(Margin(8, 2, 8, _backgroundRect.getMargin().getBottom() + 8));
		_mainLayout += &_row;
		
		// Min
		Theme::apply(&_min);
		_min.setText(std::to_wstring(_settings->min));
		
		_min.input += [this](Key key, std::optional<std::wstring_view> text) {
			if (key == Key::enter)
				changed();
		};
		
		_row += &_minTitle;
		
		// Max
		Theme::apply(&_max);
		_max.setText(std::to_wstring(_settings->max));
		
		_max.input += [this](Key key, std::optional<std::wstring_view> text) {
			if (key == Key::enter)
				changed();
		};
		
		_row += &_maxTitle;
		
		// Startup
		Theme::apply(&_startup);
		_startup.setText(std::to_wstring(_settings->startup));
		
		_startup.input += [this](Key key, std::optional<std::wstring_view> text) {
			if (key == Key::enter)
				changed();
		};
		
		_row += &_startupTitle;
		
		// Offset
		Theme::apply(&_offset);
		_offset.setText(std::to_wstring(_settings->offset));
		
		_offset.input += [this](Key key, std::optional<std::wstring_view> text) {
			if (key == Key::enter)
				changed();
		};
		
		_row += &_offsetTitle;
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
		title.setText(L"Motor fine-tuning");
		
		// Content
		vaginoz(&throttle);
		vaginoz(&noseWheel);
		vaginoz(&aileronLeft);
		vaginoz(&aileronRight);
		vaginoz(&flapLeft);
		vaginoz(&flapRight);
		vaginoz(&tailLeft);
		vaginoz(&tailRight);
	}
	
	void MotorsSettingsPage::vaginoz(MotorEditor* motorEditor) {
		motorEditor->changed += [motorEditor]() {
			motorEditor->toSettings();
			RC::getInstance().getSettings().motors.scheduleWrite();
			
			RC::getInstance().getCommunicationManager().enqueue(RemotePacketType::motorConfiguration);
		};
		
		rows += motorEditor;
	}
}