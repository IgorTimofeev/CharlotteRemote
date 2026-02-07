#include "motorsSettingsPage.h"

#include <optional>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	MotorEditor::MotorEditor(std::wstring_view title, MotorConfiguration* settings) : Titler(title), _settings(settings) {
		*this += &_mainLayout;
		
		// Reverse
		_reverse.setWidth(18);
		_reverse.setContentMargin(Margin(Theme::cornerRadius, 0, 0, 0));
		_reverse.setHorizontalAlignment(Alignment::end);
		_reverse.setCornerRadius(Theme::cornerRadius);
		
		_reverse.setDefaultBackgroundColor(&Theme::bg3);
		_reverse.setDefaultTextColor(&Theme::fg2);
		
		_reverse.setActiveBackgroundColor(&Theme::fg1);
		_reverse.setActiveTextColor(&Theme::bg1);
		
		_reverse.setFont(&Theme::fontSmall);
		_reverse.setText(L"<>");
		_reverse.setToggle(true);
		
		_reverse.setActive(_settings->reverse);
		
		_reverse.setOnClick([this]() {
			changed();
		});
		
		_mainLayout += &_reverse;
		
		// Background rect
		_backgroundRect.setMargin(Margin(0, 0, _reverse.getSize().getWidth() - Theme::cornerRadius - 1, 0));
		_backgroundRect.setCornerRadius(Theme::cornerRadius);
		_backgroundRect.setFillColor(&Theme::bg2);
		_backgroundRect.setBorderColor(&Theme::bg3);
		_mainLayout += &_backgroundRect;
		
		// Row
		_row.setOrientation(Orientation::horizontal);
		_row.setGap(8);
		_row.setMargin(Margin(8, 3, 8 + _backgroundRect.getMargin().getRight(), 8));
		_mainLayout += &_row;
		
		// Min
		addTextField(_minTitle, _min, _settings->min);
		
		// Max
		addTextField(_maxTitle, _max, _settings->max);
	}
	
	void MotorEditor::toSettings() const {
		_settings->min = static_cast<uint16_t>(StringUtils::tryParseInt32Or(_min.getText(), 1'000));
		_settings->max = static_cast<uint16_t>(StringUtils::tryParseInt32Or(_max.getText(), 2'000));
		_settings->reverse = _reverse.isActive();
		_settings->sanitize();
	}
	
	MotorsSettingsPage::MotorsSettingsPage() :
		throttle(L"Throttle", &RC::getInstance().getSettings().motors.throttle),
		noseWheel(L"Nose wheel", &RC::getInstance().getSettings().motors.noseWheel),
		
		flapLeft(L"Left flap", &RC::getInstance().getSettings().motors.flapLeft),
		aileronLeft(L"Left aileron", &RC::getInstance().getSettings().motors.aileronLeft),
		
		flapRight(L"Right flap", &RC::getInstance().getSettings().motors.flapRight),
		aileronRight(L"Right aileron", &RC::getInstance().getSettings().motors.aileronRight),
		
		tailLeft(L"Left tail", &RC::getInstance().getSettings().motors.tailLeft),
		tailRight(L"Right tail", &RC::getInstance().getSettings().motors.tailRight)
	{
		// Page title
		title.setText(L"Motors");
		
		// Content
		vaginoz(&throttle);
		vaginoz(&noseWheel);

		vaginoz(&flapLeft);
		vaginoz(&aileronLeft);
		
		vaginoz(&flapRight);
		vaginoz(&aileronRight);

		vaginoz(&tailLeft);
		vaginoz(&tailRight);
	}
	
	void MotorsSettingsPage::vaginoz(MotorEditor* motorEditor) {
		motorEditor->changed = [motorEditor]() {
			motorEditor->toSettings();
			RC::getInstance().getSettings().motors.scheduleWrite();
			
			RC::getInstance().getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::motorConfiguration);
		};
		
		rows += motorEditor;
	}
}