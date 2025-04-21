#include "MFDMenuView.h"

#include "../../theme.h"
#include "../../../rc.h"
#include "../MFD/MFDPage.h"
#include "menuRoutes.h"

namespace pizda {
	MFDModeMenuButton::MFDModeMenuButton(const std::wstring_view& text, SettingsInterfaceMFDInstrumentsMode mode) : MenuButton(text), _mode(mode) {
		setCheckMode(ButtonCheckMode::manual);
		setPressedBorderColor(&Theme::fg1);
		setChecked(RC::getInstance().getSettings().interface.MFDInstrumentsMode == mode);
	}

	void MFDModeMenuButton::onClick() {
		MenuButton::onClick();

		auto view = reinterpret_cast<MFDMenuView*>(getMenuView());

		for (auto modeButton : view->modeButtons) {
			modeButton->setChecked(modeButton == this);
		}

		RC::getInstance().getApplication().enqueueOnTick([this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFDInstrumentsMode = _mode;
			settings.enqueueWrite();

			MFDPage::fromSettings();
		});
	}

	MFDMenuView::MFDMenuView() : mainViewButton(ViewMenuButton(L"Settings", &MenuRoutes::main)) {
		title.setText(L"MFD options");

		const auto& settings = RC::getInstance().getSettings();

		// N/D
		NDButton.setPressedBorderColor(&Theme::purple);
		NDButton.setCheckMode(ButtonCheckMode::manual);
		NDButton.setChecked(settings.interface.MFDNavDisplay);

		NDButton.click += [this]() {
			NDButton.setChecked(!NDButton.isChecked());

			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFDNavDisplay = !settings.interface.MFDNavDisplay;
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