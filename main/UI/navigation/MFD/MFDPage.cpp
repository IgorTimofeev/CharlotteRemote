#include "MFDPage.h"

#include <UI/theme.h>
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

		if (_mainControls && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::main)
			_mainControls = nullptr;

		if (_autopilotControls && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::autopilot)
			_autopilotControls = nullptr;

		if (_pressureControls && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::pressure)
			_pressureControls = nullptr;

		// Creating
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

		if (_rows.getChildrenCount() > 1)
			_rows.setRelativeSize(_rows[0], static_cast<float>(settings.interface.MFD.splitPercent) / 100.f * 2.f);

		switch (settings.interface.MFD.toolbar.mode) {
			case SettingsInterfaceMFDToolbarMode::main: {
				if (!_mainControls)
					_mainControls = new MainControls();

				_rows.setAutoSize(_mainControls, true);
				_rows += _mainControls;

				break;
			}
			case SettingsInterfaceMFDToolbarMode::autopilot: {
				if (!_autopilotControls)
					_autopilotControls = new AutopilotControls();

				_rows.setAutoSize(_autopilotControls, true);
				_rows += _autopilotControls;

				break;
			}
			case SettingsInterfaceMFDToolbarMode::pressure: {
				if (!_pressureControls)
					_pressureControls = new PressureControls();

				_rows.setAutoSize(_pressureControls, true);
				_rows += _pressureControls;

				break;
			}
			default:
				break;
		}

		if (settings.interface.MFD.toolbar.mode == SettingsInterfaceMFDToolbarMode::none) {
			rc.getOpenMenuButton().applyBottomStyle();
		}
		else {
			rc.getOpenMenuButton().applyBottomRightStyle();
		}

		invalidate();
	}
}