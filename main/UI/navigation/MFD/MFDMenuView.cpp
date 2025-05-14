#include "../MFD/MFDMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"
#include "../../../settings/settings.h"

#include "../routes.h"
#include "MFDPage.h"

namespace pizda {
	MFDModeMenuViewButton::MFDModeMenuViewButton(const Image* image, std::wstring_view text, SettingsInterfaceMFDToolbarMode mode) : MenuViewButton(image, text), _mode(mode) {
		setToggle(true);
		setActive(RC::getInstance().getSettings().interface.MFD.toolbar.mode == mode);
	}

	void MFDModeMenuViewButton::onClick() {
		MenuViewButton::onClick();

		const auto menuView = reinterpret_cast<MFDMenuView*>(getMenuView());

		auto& settings = RC::getInstance().getSettings();
		settings.interface.MFD.toolbar.mode = settings.interface.MFD.toolbar.mode == _mode ? SettingsInterfaceMFDToolbarMode::none : _mode;

		for (const auto modeButton : menuView->modeButtons)
			modeButton->setActive(modeButton->_mode == settings.interface.MFD.toolbar.mode);

		MFDPage::fromSettings();

		settings.interface.scheduleWrite();
	}

	MFDMenuView::MFDMenuView() {
		auto& settings = RC::getInstance().getSettings();

		// PFD
		PFDButton.setActiveBorderColor(&Theme::purple);
		PFDButton.setActiveTextColor(&Theme::purple);
		PFDButton.setToggle(true);
		PFDButton.setActive(settings.interface.MFD.PFD.visible);

		PFDButton.click += [this, &settings] {
			settings.interface.MFD.PFD.visible = !settings.interface.MFD.PFD.visible;

			if (!settings.interface.MFD.isAnyPanelVisible())
				settings.interface.MFD.PFD.visible = true;

			PFDButton.setActive(settings.interface.MFD.PFD.visible);

			settings.interface.scheduleWrite();

			MFDPage::fromSettings();
		};

		*this += &PFDButton;

		// N/D
		NDButton.setActiveBorderColor(&Theme::purple);
		NDButton.setActiveTextColor(&Theme::purple);
		NDButton.setToggle(true);
		NDButton.setActive(settings.interface.MFD.ND.visible);

		NDButton.click += [this, &settings] {
			settings.interface.MFD.ND.visible = !settings.interface.MFD.ND.visible;

			if (!settings.interface.MFD.isAnyPanelVisible())
				settings.interface.MFD.ND.visible = true;

			NDButton.setActive(settings.interface.MFD.ND.visible);

			settings.interface.scheduleWrite();

			MFDPage::fromSettings();
		};

		*this += &NDButton;

		// Mode
		for (const auto modeButton : modeButtons) {
			*this += modeButton;
		}
	}

	const Route* MFDMenuView::getRoute() {
		return &Routes::MFD;
	}
}