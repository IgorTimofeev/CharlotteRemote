#include "MFDPage.h"

#include "../../../theme.h"
#include "../../../../rc.h"

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

		deleteControls();
	}

	void MFDPage::fromSettings() {
		if (_instance)
			_instance->fromSettingsInstance();
	}

	void MFDPage::deleteControls() {
		auto& settings = RC::getInstance().getSettings();

		if (_NDControls && !settings.interface.MFDNavDisplay) {
			delete _NDControls;
			_NDControls = nullptr;
		}

		if (_mainControls && settings.interface.MFDInstrumentsMode != SettingsInterfaceMFDInstrumentsMode::main) {
			delete _mainControls;
			_mainControls = nullptr;
		}

		if (_autopilotControls && settings.interface.MFDInstrumentsMode != SettingsInterfaceMFDInstrumentsMode::autopilot) {
			delete _autopilotControls;
			_autopilotControls = nullptr;
		}

		if (_pressureControls && settings.interface.MFDInstrumentsMode != SettingsInterfaceMFDInstrumentsMode::pressure) {
			delete _pressureControls;
			_pressureControls = nullptr;
		}
	}

	void MFDPage::fromSettingsInstance() {
		_rows.removeChildren();
		deleteControls();

		_rows += &_PFD;

		auto& settings = RC::getInstance().getSettings();

		if (settings.interface.MFDNavDisplay) {
			if (!_NDControls)
				_NDControls = new NDControls();

			_rows.setRelativeSize(_NDControls, 0.8f);
			_rows += _NDControls;
		}

		switch (settings.interface.MFDInstrumentsMode) {
			case SettingsInterfaceMFDInstrumentsMode::main: {
				if (!_mainControls)
					_mainControls = new MainControls();

				_rows.setAutoSize(_mainControls, true);
				_rows += _mainControls;

				break;
			}
			case SettingsInterfaceMFDInstrumentsMode::autopilot: {
				if (!_autopilotControls)
					_autopilotControls = new AutopilotControls();

				_rows.setAutoSize(_autopilotControls, true);
				_rows += _autopilotControls;

				break;
			}
			case SettingsInterfaceMFDInstrumentsMode::pressure: {
				if (!_pressureControls)
					_pressureControls = new PressureControls();

				_rows.setAutoSize(_pressureControls, true);
				_rows += _pressureControls;

				break;
			}
		}

		invalidate();
	}
}