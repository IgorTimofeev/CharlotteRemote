#include "MFDMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"
#include "../../../settings.h"

#include "../routes.h"
#include "../MFD/MFDPage.h"

namespace pizda {
	MFDModeMenuViewButton::MFDModeMenuViewButton(const Image* image, std::wstring_view text, SettingsInterfaceMFDToolbarMode mode) : MenuViewButton(image, text), _mode(mode) {
		setCheckMode(ButtonCheckMode::manual);
		setChecked(RC::getInstance().getSettings().interface.MFD.toolbar.mode == mode);
	}

	void MFDModeMenuViewButton::onClick() {
		MenuViewButton::onClick();

		const auto menuView = reinterpret_cast<MFDMenuView*>(getMenuView());

		for (const auto modeButton : menuView->modeButtons)
			modeButton->setChecked(modeButton->_mode == _mode);

		auto& settings = RC::getInstance().getSettings();
		settings.interface.MFD.toolbar.mode = _mode;
		settings.enqueueWrite();

		MFDPage::fromSettings();
	}

	MFDMenuView::MFDMenuView() {
		auto& settings = RC::getInstance().getSettings();

		// PFD
		PFDButton.setPressedBorderColor(&Theme::purple);
		PFDButton.setPressedTextColor(&Theme::purple);
		PFDButton.setCheckMode(ButtonCheckMode::manual);
		PFDButton.setChecked(settings.interface.MFD.PFD.visible);

		PFDButton.click += [this, &settings] {
			settings.interface.MFD.PFD.visible = !settings.interface.MFD.PFD.visible;

			if (!settings.interface.MFD.isAnyPanelVisible())
				settings.interface.MFD.PFD.visible = true;

			PFDButton.setChecked(settings.interface.MFD.PFD.visible);

			settings.enqueueWrite();

			MFDPage::fromSettings();
		};

		*this += &PFDButton;

		// N/D
		NDButton.setPressedBorderColor(&Theme::purple);
		NDButton.setPressedTextColor(&Theme::purple);
		NDButton.setCheckMode(ButtonCheckMode::manual);
		NDButton.setChecked(settings.interface.MFD.ND.visible);

		NDButton.click += [this, &settings] {
			settings.interface.MFD.ND.visible = !settings.interface.MFD.ND.visible;

			if (!settings.interface.MFD.isAnyPanelVisible())
				settings.interface.MFD.ND.visible = true;

			NDButton.setChecked(settings.interface.MFD.ND.visible);

			settings.enqueueWrite();

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