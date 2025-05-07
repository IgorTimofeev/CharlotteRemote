#include "MFDMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"
#include "../../../settings.h"

#include "../routes.h"
#include "../MFD/MFDPage.h"

namespace pizda {
	MFDModeMenuViewButton::MFDModeMenuViewButton(const Image* image, const std::wstring_view& text, SettingsInterfaceMFDInstrumentsMode mode) : MenuViewButton(image, text), _mode(mode) {
		setCheckMode(ButtonCheckMode::manual);
		setChecked(RC::getInstance().getSettings().interface.MFD.instrumentsMode == mode);
	}

	void MFDModeMenuViewButton::onClick() {
		MenuViewButton::onClick();

		const auto menuView = reinterpret_cast<MFDMenuView*>(getMenuView());

		for (auto modeButton : menuView->modeButtons) {
			modeButton->setChecked(modeButton == this);
		}

		auto& settings = RC::getInstance().getSettings();
		settings.interface.MFD.instrumentsMode = _mode;
		settings.enqueueWrite();

		MFDPage::fromSettings();
	}

	MFDMenuView::MFDMenuView() {
		const auto& settings = RC::getInstance().getSettings();

		// N/D
		NDButton.setPressedBorderColor(&Theme::purple);
		NDButton.setPressedTextColor(&Theme::purple);

		NDButton.setCheckMode(ButtonCheckMode::manual);
		NDButton.setChecked(settings.interface.MFD.ND.show);

		NDButton.click += [this]() {
			NDButton.setChecked(!NDButton.isChecked());

			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFD.ND.show = !settings.interface.MFD.ND.show;
			settings.enqueueWrite();

			MFDPage::fromSettings();
		};

		*this += &NDButton;

		// Mode
		for (auto modeButton : modeButtons) {
			*this += modeButton;
		}
	}

	const Route* MFDMenuView::getRoute() {
		return &Routes::MFD;
	}
}