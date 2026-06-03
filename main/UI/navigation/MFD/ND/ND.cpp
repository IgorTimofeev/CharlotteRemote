#include "ND.h"
#include "rc.h"
#include "UI/elements/navigation/addWaypointDialog.h"
#include "UI/navigation/MFD/MFDPage.h"

namespace pizda {
	NDActionButtonDialog::NDActionButtonDialog(NDScene* scene) {
		auto& rc = RC::getInstance();

		title.setText("Coord");

		// Create waypoint
		Theme::applyPrimary(&_newWaypointButton);
		_newWaypointButton.setText("Create waypoint");

		_newWaypointButton.setOnClick([this,  scene, &rc] {
			rc.getApplication().invokeLater([this, scene] {
				AddWaypointDialog::create(scene->getCameraCoordinates(), [scene] {
					scene->deleteSceneElements();
					scene->createSceneElementsFromNavigationData();
					scene->setFocused(true);
				});

				hide();
				delete this;
			});
		});

		rows += &_newWaypointButton;

		// Set home
		Theme::applySecondary(&_setHomeButton);
		_setHomeButton.setText("Set home");

		_setHomeButton.setOnClick([this, &rc, scene] {
			rc.getApplication().invokeLater([&rc, this, scene] {
				scene->resetCameraLateralOffset();
				scene->setFocused(true);

				rc.getRemoteData().homeCoordinates = scene->getCameraCoordinates();
				rc.getTransceiver().enqueueSystemPacket(RemoteSystemPacketType::homeCoordinates);

				hide();
				delete this;
			});
		});

		rows += &_setHomeButton;
	}

	ND::ND() {
		setClipToBounds(true);

		// Scene
		*this += &scene;

		// ----------------------------- Left buttons -----------------------------

		// Button rows
		_buttonRows.setGap(1);
		_buttonRows.setAlignment(Alignment::start, Alignment::center);
		*this += &_buttonRows;

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

			scene.setFocused(true);
		});

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

			scene.setFocused(true);
		});

		// RST
		_latLongButton.setText("RST");

		addGovnoButton(&_latLongButton, [this] {
			scene.resetCameraLateralOffset();
			scene.setFocused(true);
		});

		// +
		_actionButton.setText("+");

		addGovnoButton(&_actionButton, [this] {
			const auto dialog = new NDActionButtonDialog(&scene);
			dialog->show();
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
		std::string text;

		switch (RC::getInstance().getSettings().personalization.MFD.ND.mode) {
			case PersonalizationSettingsMFDNDMode::arc:
				text = "ARC";
				break;
			case PersonalizationSettingsMFDNDMode::map:
				text = "MAP";
				break;
		}

		_viewModeButton.setText(text);
	}

	void ND::updateSplitButtonText() {
		_splitButton.setText(RC::getInstance().getSettings().personalization.MFD.split.mode == PersonalizationSettingsMFDSplitMode::ND ? "> <" : "< >");
	}

}
