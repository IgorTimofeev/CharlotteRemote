#include "motorsSettingsPage.h"

#include <optional>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	MotorEditor::MotorEditor(const std::string_view title, const MotorType type) : Titler(title), _type(type) {
		const auto settings = RC::getInstance().getSettings().motors.getByType(type);

		*this += &_mainLayout;
		
		// Reverse
		_reverse.setWidth(20);
		_reverse.setContentMargin(Margin(Theme::cornerRadius - 1, 0, 0, 0));
		_reverse.setHorizontalAlignment(Alignment::end);
		_reverse.setCornerRadius(Theme::cornerRadius);
		
		_reverse.setDefaultBackgroundColor(&Theme::bg2);
		_reverse.setDefaultTextColor(&Theme::fg4);
		
		_reverse.setActiveBackgroundColor(&Theme::fg1);
		_reverse.setActiveTextColor(&Theme::bg1);
		
		_reverse.setFont(&Theme::fontSmall);
		_reverse.setText("<>");
		_reverse.setToggle(true);
		
		_reverse.setActive(settings->reverse);
		
		_reverse.setOnClick([this] {
			changed();
		});
		
		_mainLayout += &_reverse;
		
		// Row
		_minMaxRow.setOrientation(Orientation::horizontal);
		_minMaxRow.setGap(8);
		_minMaxRow.setMargin(Margin(0, 0, _reverse.getSize().getWidth() - Theme::cornerRadius - 1, 0));
		_mainLayout += &_minMaxRow;
		
		// Min
		addTextField(_min, settings->min);
		
		// Max
		addTextField(_max, settings->max);
	}

	void MotorEditor::changed() const {
		auto& rc = RC::getInstance();

		rc.getRemoteData().motorSettings.type = _type;
		rc.getRemoteData().motorSettings.settings.min = static_cast<uint16_t>(StringUtils::tryParseInt32Or(_min.getText(), 1'000));
		rc.getRemoteData().motorSettings.settings.max = static_cast<uint16_t>(StringUtils::tryParseInt32Or(_max.getText(), 2'000));
		rc.getRemoteData().motorSettings.settings.reverse = _reverse.isActive();
		rc.getRemoteData().motorSettings.settings.sanitize();

		*rc.getSettings().motors.getByType(_type) = rc.getRemoteData().motorSettings.settings;
		rc.getSettings().motors.scheduleWrite();

		rc.getTransceiver().enqueueSystemPacket(RemoteSystemPacketType::motors);
	}

	MotorsSettingsPage::MotorsSettingsPage() {
		// Page title
		title.setText("Primary");
		
		vaginoz(&_throttle);
		// vaginoz(&_reverse);
		vaginoz(&_noseWheel);
		rows += &_mainSeparator;

		penisula(&_leftWingTitle);
		vaginoz(&_flapLeft);
		vaginoz(&_aileronLeft);
		rows += &_leftWingSeparator;

		penisula(&_rightWingTitle);
		vaginoz(&_flapRight);
		vaginoz(&_aileronRight);
		rows += &_rightWingSeparator;

		penisula(&_tailTitle);
		vaginoz(&_tailLeft);
		vaginoz(&_tailRight);
		rows += &_tailSeparator;

		penisula(&_cameraTitle);
		vaginoz(&_cameraPitch);
		vaginoz(&_cameraYaw);
	}

	void MotorsSettingsPage::penisula(TextView* text) {
		Theme::applyPageTitle(text);

		rows += text;
	}

	void MotorsSettingsPage::vaginoz(MotorEditor* motorEditor) {
		rows += motorEditor;
	}
}