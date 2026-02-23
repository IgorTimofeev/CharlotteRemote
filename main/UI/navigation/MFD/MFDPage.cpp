#include "MFDPage.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	void MFDLayout::deleteShit() {
		removeChildren();

		auto& rc = RC::getInstance();

		if (_PFD.get() && rc.getSettings().personalization.MFD.split.mode == PersonalizationSettingsMFDSplitMode::ND)
			_PFD.reset();

		if (_ND.get() && rc.getSettings().personalization.MFD.split.mode == PersonalizationSettingsMFDSplitMode::PFD)
			_ND.reset();
	}

	void MFDLayout::createShit() {
		auto& rc = RC::getInstance();

		if (rc.getSettings().personalization.MFD.split.mode != PersonalizationSettingsMFDSplitMode::ND) {
			if (!_PFD)
				_PFD = std::make_unique<PFD>();

			*this += _PFD.get();
		}

		if (rc.getSettings().personalization.MFD.split.mode != PersonalizationSettingsMFDSplitMode::PFD) {
			if (!_ND)
				_ND = std::make_unique<ND>();

			*this += _ND.get();
		}

		updateSplitRatio();
	}

	void MFDLayout::updateSplitRatio() {
		auto& rc = RC::getInstance();

		if (_PFD.get() && _ND.get()) {
			setRelativeSize(_PFD.get(), static_cast<float>(rc.getSettings().personalization.MFD.split.ratio) / 100.f);
			setRelativeSize(_ND.get(), 1.f - static_cast<float>(rc.getSettings().personalization.MFD.split.ratio) / 100.f);
		}
		else if (_PFD.get()) {
			setRelativeSize(_PFD.get(), 1);
		}
	}

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

	MFDPage* MFDPage::getInstance() {
		return _instance;
	}

	MFDLayout& MFDPage::getMFDLayout() {
		return _MFDLayout;
	}

	void MFDPage::fromSettings() {
		_rows.removeChildren();

		auto& rc = RC::getInstance();

		// Deleting
		_MFDLayout.deleteShit();

		if (_autopilotToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::autopilot)
			_autopilotToolbar.reset();

		if (_baroToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::baro)
			_baroToolbar.reset();
		
		if (_trimToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::trim)
			_trimToolbar.reset();
		
		if (_lightsToolbar.get() && rc.getSettings().personalization.MFD.toolbar.mode != PersonalizationSettingsMFDToolbarMode::lights)
			_lightsToolbar.reset();
		
		// Creating
		switch (rc.getSettings().personalization.MFD.toolbar.mode) {
			case PersonalizationSettingsMFDToolbarMode::autopilot: {
				if (!_autopilotToolbar)
					_autopilotToolbar = std::make_unique<AutopilotToolbar>();
				
				_rows.setAutoSize(_autopilotToolbar.get(), true);
				_rows += _autopilotToolbar.get();
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::baro: {
				if (!_baroToolbar)
					_baroToolbar = std::make_unique<BaroToolbar>();
				
				_rows.setAutoSize(_baroToolbar.get(), true);
				_rows += _baroToolbar.get();
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::trim: {
				if (!_trimToolbar)
					_trimToolbar = std::make_unique<TrimToolbar>();
				
				_rows.setAutoSize(_trimToolbar.get(), true);
				_rows += _trimToolbar.get();
				
				break;
			}
			case PersonalizationSettingsMFDToolbarMode::lights: {
				if (!_lightsToolbar)
					_lightsToolbar = std::make_unique<LightsToolbar>();
				
				_rows.setAutoSize(_lightsToolbar.get(), true);
				_rows += _lightsToolbar.get();
				
				break;
			}
			default:
				break;
		}

		_MFDLayout.createShit();
		_rows += &_MFDLayout;
		
		// Main
		_rows.setAutoSize(&_mainToolbar, true);
		_rows += &_mainToolbar;
		
		invalidate();
	}
}
