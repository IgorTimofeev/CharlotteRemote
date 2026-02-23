#include "ND.h"
#include "rc.h"
#include "UI/elements/navigation/addWaypointDialog.h"
#include "UI/navigation/MFD/MFDPage.h"

namespace pizda {
	ND::ND() {
		setClipToBounds(true);

		// Scene
		*this += &_scene;

		// Button rows
		_buttonRows.setGap(1);
		_buttonRows.setAlignment(Alignment::start, Alignment::center);
		*this += &_buttonRows;

		// View mode
		updateViewModeButtonText();

		addGovnoButton(&_viewModeButton, [this] {
			auto& settings = RC::getInstance().getSettings().personalization;

			uint8_t nextMode = static_cast<uint8_t>(settings.MFD.ND.mode) + 1;

			if (nextMode > static_cast<uint8_t>(PersonalizationSettingsMFDNDMode::maxValue))
				nextMode = 0;

			settings.MFD.ND.mode = static_cast<PersonalizationSettingsMFDNDMode>(nextMode);
			settings.scheduleWrite();

			updateViewModeButtonText();

			_scene.setFocused(true);
		});

		// RST
		_latLongButton.setText(L"RST");

		addGovnoButton(&_latLongButton, [this] {
			_scene.resetCameraLateralOffset();
			_scene.setFocused(true);
		});

		// +
		_waypointButton.setText(L"WPT");

		addGovnoButton(&_waypointButton, [this] {
			AddWaypointDialog::create(_scene.getCameraCoordinates(), [this] {
				_scene.deleteSceneElements();
				_scene.createSceneElementsFromNavigationData();
				_scene.setFocused(true);
			});
		});

		// Split
		updateSplitButtonText();

		addGovnoButton(&_splitButton, [this] {
			auto& settings = RC::getInstance().getSettings().personalization;

			settings.MFD.split.mode =
				settings.MFD.split.mode == PersonalizationSettingsMFDSplitMode::split
				? PersonalizationSettingsMFDSplitMode::ND
				: PersonalizationSettingsMFDSplitMode::split;

			settings.scheduleWrite();

			updateSplitButtonText();
			MFDPage::getInstance()->fromSettings();

			_scene.setFocused(true);
		});
	}

	void ND::addGovnoButton(Button* button, const std::function<void()>& onClick) {
		button->setSize(Size(PFD::speedWidth, 20));
		button->setCornerRadius(0);

		button->setDefaultBackgroundColor(&Theme::bg2);
		button->setDefaultTextColor(&Theme::fg5);

		button->setActiveBackgroundColor(&Theme::fg1);
		button->setActiveTextColor(&Theme::bg1);

		button->setFont(&Theme::fontSmall);

		button->setOnClick(onClick);

		_buttonRows += button;
	}

	void ND::updateViewModeButtonText() {
		std::wstring text;

		switch (RC::getInstance().getSettings().personalization.MFD.ND.mode) {
			case PersonalizationSettingsMFDNDMode::arc:
				text = L"ARC";
				break;
			case PersonalizationSettingsMFDNDMode::map:
				text = L"MAP";
				break;
		}

		_viewModeButton.setText(text);
	}

	void ND::updateSplitButtonText() {
		_splitButton.setText(RC::getInstance().getSettings().personalization.MFD.split.mode == PersonalizationSettingsMFDSplitMode::ND ? L"> <" : L"< >");
	}

}
