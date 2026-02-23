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

		MFDPage::getInstance()->fromSettings();

		settings.personalization.scheduleWrite();
	}

	MFDMenuView::MFDMenuView() {
		for (const auto modeButton : modeButtons) {
			*this += *modeButton;
		}
	}

	const Route* MFDMenuView::getRoute() {
		return &Routes::MFD;
	}
}