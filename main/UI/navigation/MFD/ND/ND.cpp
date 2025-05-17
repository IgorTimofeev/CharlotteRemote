#include "ND.h"
#include <rc.h>
#include <UI/elements/dialogs/addWaypointDialog.h>

namespace pizda {
	ND::ND() {
		*this += &_scene;

		// Right rows
		_rightRows.setSpacing(10);
		_rightRows.setAlignment(Alignment::end, Alignment::start);
		_rightRows.setMargin(Margin(0, 5, 5, 0));
		*this += &_rightRows;

		updateViewModeButtonText();

		addGovnoButton(&_viewModeButton, [this] {
			auto& settings = RC::getInstance().getSettings();

			uint8_t nextMode = static_cast<uint8_t>(settings.interface.MFD.ND.mode) + 1;

			if (nextMode > static_cast<uint8_t>(SettingsInterfaceMFDNDMode::last))
				nextMode = 0;

			settings.interface.MFD.ND.mode = static_cast<SettingsInterfaceMFDNDMode>(nextMode);
			settings.interface.scheduleWrite();

			updateViewModeButtonText();

			_scene.setFocused(true);
		});

		_latLongButton.setText(L"RST");

		addGovnoButton(&_latLongButton, [this] {
			_scene.resetCameraLateralOffset();
			_scene.setFocused(true);
		});

		_waypointButton.setText(L"+");

		addGovnoButton(&_waypointButton, [this] {
			AddWaypointDialog::create(_scene.getCameraCoordinates(), [this] {
				_scene.deleteSceneElements();
				_scene.createSceneElementsFromNavigationData();
				_scene.setFocused(true);
			});
		});
	}

	void ND::addGovnoButton(Button* button, const std::function<void()>& onClick) {
		Theme::apply(button);

		button->setSize(Size(20));

		button->setDefaultBackgroundColor(&Theme::bg2);
		button->setDefaultTextColor(&Theme::fg5);

		button->setActiveBackgroundColor(&Theme::fg1);
		button->setActiveTextColor(&Theme::bg1);

		button->setFont(&Theme::fontSmall);

		button->click += onClick;

		_rightRows += button;
	}

	void ND::updateViewModeButtonText() {
		std::wstring text;

		switch (RC::getInstance().getSettings().interface.MFD.ND.mode) {
			case SettingsInterfaceMFDNDMode::arcHeadingUp:
				text = L"ARC";
				break;
			case SettingsInterfaceMFDNDMode::mapHeadingUp:
				text = L"MAP";
				break;
			case SettingsInterfaceMFDNDMode::mapNorthUp:
				text = L"NUP";
				break;
		}

		_viewModeButton.setText(text);
	}
}