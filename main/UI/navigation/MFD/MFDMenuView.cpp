#include "MFDMenuView.h"

#include "UI/theme.h"
#include "rc.h"
#include "settings/settings.h"

#include "UI/navigation/routes.h"
#include "MFDPage.h"

namespace pizda {
	MFDModeMenuViewButton::MFDModeMenuViewButton(const Image* image, const std::wstring_view text, const PersonalizationSettingsMFDToolbarMode mode) : MenuViewButton(image, text), _mode(mode) {
		setToggle(true);
		setActive(RC::getInstance().getSettings().personalization.MFD.toolbar.mode == mode);
	}

	void MFDModeMenuViewButton::onClick() {
		MenuViewButton::onClick();

		const auto menuView = reinterpret_cast<MFDMenuView*>(getMenuView());

		auto& settings = RC::getInstance().getSettings();
		settings.personalization.MFD.toolbar.mode = settings.personalization.MFD.toolbar.mode == _mode ? PersonalizationSettingsMFDToolbarMode::none : _mode;

		for (const auto modeButton : menuView->modeButtons)
			modeButton->setActive(modeButton->_mode == settings.personalization.MFD.toolbar.mode);

		MFDPage::fromSettings();

		settings.personalization.scheduleWrite();
	}

	MFDMenuView::MFDMenuView() {
		auto& settings = RC::getInstance().getSettings();

		// PFD
		PFDButton.setActiveBorderColor(&Theme::magenta1);
		PFDButton.setActiveTextColor(&Theme::magenta1);
		PFDButton.setToggle(true);
		PFDButton.setActive(settings.personalization.MFD.PFD.visible);

		PFDButton.setOnClick([this, &settings] {
			settings.personalization.MFD.PFD.visible = !settings.personalization.MFD.PFD.visible;

			if (!settings.personalization.MFD.isAnyPanelVisible())
				settings.personalization.MFD.PFD.visible = true;

			PFDButton.setActive(settings.personalization.MFD.PFD.visible);

			settings.personalization.scheduleWrite();

			MFDPage::fromSettings();
		});

		*this += &PFDButton;

		// N/D
		NDButton.setActiveBorderColor(&Theme::magenta1);
		NDButton.setActiveTextColor(&Theme::magenta1);
		NDButton.setToggle(true);
		NDButton.setActive(settings.personalization.MFD.ND.visible);

		NDButton.setOnClick([this, &settings] {
			settings.personalization.MFD.ND.visible = !settings.personalization.MFD.ND.visible;

			if (!settings.personalization.MFD.isAnyPanelVisible())
				settings.personalization.MFD.ND.visible = true;

			NDButton.setActive(settings.personalization.MFD.ND.visible);

			settings.personalization.scheduleWrite();

			MFDPage::fromSettings();
		});

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