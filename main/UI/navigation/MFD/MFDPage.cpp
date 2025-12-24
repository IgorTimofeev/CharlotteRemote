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
		if (_PFD.get() && !settings.interface.MFD.PFD.visible)
			_PFD.reset();

		if (_ND.get() && !settings.interface.MFD.ND.visible)
			_ND.reset();

		if (_mainControls.get() && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::main)
			_mainControls.reset();

		if (_autopilotControls.get() && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::autopilot)
			_autopilotControls.reset();

		if (_pressureControls.get() && settings.interface.MFD.toolbar.mode != SettingsInterfaceMFDToolbarMode::pressure)
			_pressureControls.reset();

		// Creating
		if (settings.interface.MFD.PFD.visible) {
			if (!_PFD.get())
				_PFD = std::make_unique<PFD>();

			_rows += _PFD.get();
		}

		if (settings.interface.MFD.ND.visible) {
			if (!_ND.get())
				_ND = std::make_unique<ND>();

			_rows += _ND.get();
		}

		if (_rows.getChildrenCount() > 1)
			_rows.setRelativeSize(_rows[0], static_cast<float>(settings.interface.MFD.splitPercent) / 100.f * 2.f);

		switch (settings.interface.MFD.toolbar.mode) {
			case SettingsInterfaceMFDToolbarMode::main: {
				if (!_mainControls.get())
					_mainControls = std::make_unique<MainControls>();

				_rows.setAutoSize(_mainControls.get(), true);
				_rows += _mainControls.get();

				break;
			}
			case SettingsInterfaceMFDToolbarMode::autopilot: {
				if (!_autopilotControls.get())
					_autopilotControls = std::make_unique<AutopilotControls>();

				_rows.setAutoSize(_autopilotControls.get(), true);
				_rows += _autopilotControls.get();

				break;
			}
			case SettingsInterfaceMFDToolbarMode::pressure: {
				if (!_pressureControls.get())
					_pressureControls = std::make_unique<PressureControls>();

				_rows.setAutoSize(_pressureControls.get(), true);
				_rows += _pressureControls.get();

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