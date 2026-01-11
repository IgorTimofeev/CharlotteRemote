#include "MFDPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	MFDPage* MFDPage::_instance = nullptr;

	MFDPage::MFDPage() {
		_instance = this;

		*this += &_rows;

		// Initialization
		fromSettings();
	}

	MFDPage::~MFDPage() {
		_instance = nullptr;
	}

	void MFDPage::fromSettings() {
		if (_instance)
			_instance->fromSettingsInstance();
	}

	void MFDPage::fromSettingsInstance() {
		_rows.removeChildren();

		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();

		// Deleting
		if (_PFD && !settings.personalization.MFD.PFD.visible)
			_PFD = nullptr;

		if (_ND && !settings.personalization.MFD.ND.visible)
			_ND = nullptr;
		
		if (_autopilotToolbar && settings.personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::autopilot)
			_autopilotToolbar = nullptr;

		if (_baroToolbar && settings.personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::baro)
			_baroToolbar = nullptr;
		
		if (_trimToolbar && settings.personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::trim)
			_trimToolbar = nullptr;
		
		if (_lightsToolbar && settings.personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::lights)
			_lightsToolbar = nullptr;
		
		// Creating
		switch (settings.personalization.MFD.toolbar.mode) {
			case PersonalizationSettingsMFDToolbarMode::autopilot: {
				if (!_autopilotToolbar)
					_autopilotToolbar = new AutopilotToolbar();
				
				_rows.setAutoSize(_autopilotToolbar, true);
				_rows += _autopilotToolbar;
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::baro: {
				if (!_baroToolbar)
					_baroToolbar = new BaroToolbar();
				
				_rows.setAutoSize(_baroToolbar, true);
				_rows += _baroToolbar;
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::trim: {
				if (!_trimToolbar)
					_trimToolbar = new TrimToolbar();
				
				_rows.setAutoSize(_trimToolbar, true);
				_rows += _trimToolbar;
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::lights: {
				if (!_lightsToolbar)
					_lightsToolbar = new LightsToolbar();
				
				_rows.setAutoSize(_lightsToolbar, true);
				_rows += _lightsToolbar;
				
				break;
			}
			default:
				break;
		}

		if (settings.personalization.MFD.PFD.visible) {
			if (!_PFD)
				_PFD = new PFD();

			_rows += _PFD;
		}

		if (settings.personalization.MFD.ND.visible) {
			if (!_ND)
				_ND = new ND();

			_rows += _ND;
		}

		if (settings.personalization.MFD.PFD.visible && settings.personalization.MFD.ND.visible) {
			_rows.setRelativeSize(_PFD, static_cast<float>(settings.personalization.MFD.splitPercent) / 100.f);
			_rows.setRelativeSize(_ND, static_cast<float>(100 - settings.personalization.MFD.splitPercent) / 100.f);
		}
		
		// Main
		_rows.setAutoSize(&_mainToolbar, true);
		_rows += &_mainToolbar;
		
		invalidate();
	}
}