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
		if (_PFD && !settings.interface.MFD.PFD.visible)
			_PFD = nullptr;

		if (_ND && !settings.interface.MFD.ND.visible)
			_ND = nullptr;
		
		if (_autopilotToolbar && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::autopilot)
			_autopilotToolbar = nullptr;

		if (_baroToolbar && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::baro)
			_baroToolbar = nullptr;
		
		if (_lightsToolbar && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::lights)
			_lightsToolbar = nullptr;
		
		if (_trimToolbar && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::trim)
			_trimToolbar = nullptr;
		
		// Creating
		switch (settings.interface.MFD.toolbar.mode) {
			case SettingsInterfaceMFDToolbarMode::autopilot: {
				if (!_autopilotToolbar)
					_autopilotToolbar = new AutopilotToolbar();
				
				_rows.setAutoSize(_autopilotToolbar, true);
				_rows += _autopilotToolbar;
				
				break;
			}
			case SettingsInterfaceMFDToolbarMode::baro: {
				if (!_baroToolbar)
					_baroToolbar = new BaroToolbar();
				
				_rows.setAutoSize(_baroToolbar, true);
				_rows += _baroToolbar;
				
				break;
			}
			case SettingsInterfaceMFDToolbarMode::lights: {
				if (!_lightsToolbar)
					_lightsToolbar = new LightsToolbar();
				
				_rows.setAutoSize(_lightsToolbar, true);
				_rows += _lightsToolbar;
				
				break;
			}
			case SettingsInterfaceMFDToolbarMode::trim: {
				if (!_trimToolbar)
					_trimToolbar = new TrimToolbar();
				
				_rows.setAutoSize(_trimToolbar, true);
				_rows += _trimToolbar;
				
				break;
			}
			default:
				break;
		}

		if (settings.interface.MFD.PFD.visible) {
			if (!_PFD)
				_PFD = new PFD();

			_rows += _PFD;
		}

		if (settings.interface.MFD.ND.visible) {
			if (!_ND)
				_ND = new ND();

			_rows += _ND;
		}

		if (settings.interface.MFD.PFD.visible && settings.interface.MFD.ND.visible)
			_rows.setRelativeSize(_PFD, static_cast<float>(settings.interface.MFD.splitPercent) / 100.f * 2.f);
		
		// Main
		_rows.setAutoSize(&_mainToolbar, true);
		_rows += &_mainToolbar;
		
		invalidate();
	}
}