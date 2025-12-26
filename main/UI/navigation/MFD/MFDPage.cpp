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
		
		if (_autopilotControls && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::autopilot)
			_autopilotControls = nullptr;

		if (_baroControls && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::baro)
			_baroControls = nullptr;

		// Creating
		switch (settings.interface.MFD.toolbar.mode) {
			case SettingsInterfaceMFDToolbarMode::autopilot: {
				if (!_autopilotControls)
					_autopilotControls = new AutopilotControls();
				
				_rows.setAutoSize(_autopilotControls, true);
				_rows += _autopilotControls;
				
				break;
			}
			case SettingsInterfaceMFDToolbarMode::baro: {
				if (!_baroControls)
					_baroControls = new BaroControls();
				
				_rows.setAutoSize(_baroControls, true);
				_rows += _baroControls;
				
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

//		if (settings.interface.MFD.PFD.visible && settings.interface.MFD.ND.visible)
//			_rows.setRelativeSize(_PFD, static_cast<float>(settings.interface.MFD.splitPercent) / 100.f * 2.f);
		
		// Main
		_rows.setAutoSize(&_mainControls, true);
		_rows += &_mainControls;
		
//		if (settings.interface.MFD.toolbar.mode == SettingsInterfaceMFDToolbarMode::none) {
//			rc.getOpenMenuButton().applyBottomStyle();
//		}
//		else {
			rc.getOpenMenuButton().applyBottomRightStyle();
//		}

		invalidate();
	}
}