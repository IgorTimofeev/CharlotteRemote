#include "MFDMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"
#include "../MFD/MFDPage.h"
#include "menuRoutes.h"

namespace pizda {
	MFDModeMenuButton::MFDModeMenuButton(const Image* image, const std::wstring_view& text, SettingsInterfaceMFDInstrumentsMode mode) : MenuButton(image, text), _mode(mode) {
		setCheckMode(ButtonCheckMode::manual);
		setChecked(RC::getInstance().getSettings().interface.MFD.instrumentsMode == mode);
	}

	void MFDModeMenuButton::onClick() {
		MenuButton::onClick();

		auto view = reinterpret_cast<MFDMenuView*>(getMenuView());

		for (auto modeButton : view->modeButtons) {
			modeButton->setChecked(modeButton == this);
		}

		RC::getInstance().getApplication().enqueueOnTick([this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFD.instrumentsMode = _mode;
			settings.enqueueWrite();

			MFDPage::fromSettings();
		});
	}

	MFDMenuView::MFDMenuView() : mainViewButton(ViewMenuButton(&resources::Images::menuIconSettings, L"Settings", &MenuRoutes::main)) {
		title.setText(L"MFD options");

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

		wrapLayout += &NDButton;

		// Mode
		for (auto modeButton : modeButtons) {
			wrapLayout += modeButton;
		}

		// Main
		wrapLayout += &mainViewButton;
	}
}