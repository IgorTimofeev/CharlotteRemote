#include "mainControls.h"
#include "../../../../../rc.h"

namespace pizda {
	const PFDLandingGearRetractedImage MainControls::_landingGearRetractedImage = {};
	const PFDLandingGearExtendedImage MainControls::_landingGearExtendedImage = {};

	MainControls::MainControls() {
		const uint16_t titleHeight = Theme::fontSmall.getHeight() + titleVerticalOffset * 2;
		const uint16_t contentHeight = panelSize - titleHeight - contentVerticalOffset * 2;

		// Titles
		_titlesRect.setFillColor(&Theme::bg2);
		_titlesRect.setHeight(titleHeight);
		_titlesRect.setVerticalAlignment(Alignment::start);
		*this += &_titlesRect;

		// Row
		_row.setOrientation(Orientation::horizontal);
		_row.setSpacing(15);
		_row.setHorizontalAlignment(Alignment::center);
		_row.setMargin(Margin(8, titleVerticalOffset, 8, 0));

		// Throttle
		_throttleRow.setOrientation(Orientation::horizontal);
		_throttleRow.setSpacing(10);

		_throttle1Indicator.setSize(Size(7, contentHeight + 2));
		_throttleRow += &_throttle1Indicator;

		_throttle2Indicator.setSize(_throttle1Indicator.getSize());
		_throttleRow += &_throttle2Indicator;

		setTitlerStyle(_throttleTitle);
		_row += &_throttleTitle;

		// Controls
		_controlsRows.setSpacing(5);
		_controlsRows += &_flapsAndSpoilersIndicator;
		_controlsRows += &_landingGearImageView;

		setTitlerStyle(_controlsTitle);
		_row += &_controlsTitle;

		// Elevator trim
		_elevatorTrimIndicator.setSize(Size(5, contentHeight));
		_elevatorTrimIndicator.setSuggestedMinimum(0xFFFF * 40 / 100);
		_elevatorTrimIndicator.setSuggestedMaximum(0xFFFF * 60 / 100);

		setTitlerStyle(_elevatorTrimTitle);
		_row += &_elevatorTrimTitle;

		// Battery
		_batteryIndicatorController.setSize(Size(32, 12));
		_batteryIndicatorAircraft.setSize(_batteryIndicatorController.getSize());

		_batteryIndicatorRows.setSpacing(5);
		_batteryIndicatorRows += &_batteryIndicatorController;
		_batteryIndicatorRows += &_batteryIndicatorAircraft;

		setTitlerStyle(_batteryIndicatorTitle);
		_row += &_batteryIndicatorTitle;

		*this += &_row;
	}

	void MainControls::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// Throttles
		_throttle1Indicator.setValue(settings.controls.throttles[0]);
		_throttle2Indicator.setValue(settings.controls.throttles[1]);

		// Controls
		_landingGearImageView.setImage(
			settings.controls.landingGear
			? reinterpret_cast<const Image*>(&_landingGearExtendedImage)
			: reinterpret_cast<const Image*>(&_landingGearRetractedImage)
		);

		// Trim
		_elevatorTrimIndicator.setValue(settings.controls.elevatorTrim);

		// Battery
		_batteryIndicatorController.setVoltage(rc.getBattery().getVoltage());
		_batteryIndicatorController.setCharge(rc.getBattery().getCharge());

		_batteryIndicatorAircraft.setVoltage(28000);
		_batteryIndicatorAircraft.setCharge(0xFF);
	}

	void MainControls::onEvent(Event* event) {
		Layout::onEvent(event);

		if (event->getTypeID() == TouchDownEvent::typeID) {
			this->setFocused(true);
			event->setHandled(true);
		}
		else if (event->getTypeID() == EncoderRotateEvent::typeID) {
			if (isFocused()) {
				auto rotateEvent = (EncoderRotateEvent*) event;

				ESP_LOGI("Encoder", "RPS: %ld", rotateEvent->getRPS());

				auto& settings = RC::getInstance().getSettings();

				// Throttle
				settings.controls.throttles[0] = yoba::addSaturating(settings.controls.throttles[0], rotateEvent->getRPSFactor(60, 1, 10) * 0xFFFF / 100);
				settings.controls.throttles[1] = settings.controls.throttles[0];

				event->setHandled(true);
			}
		}
		else if (event->getTypeID() == EncoderPushEvent::typeID) {
			if (isFocused()) {
				auto pushEvent = (EncoderPushEvent*) event;

				ESP_LOGI("Encoder", "Push: %d", pushEvent->isDown());

				if (pushEvent->isDown()) {
					auto& settings = RC::getInstance().getSettings();

					settings.controls.landingGear = !settings.controls.landingGear;
				}

				event->setHandled(true);
			}
		}
	}

	void MainControls::setTitlerStyle(Titler& titler) {
		titler.setSpacing(titleSpacing);
		titler.getTitle().setFont(&Theme::fontSmall);
	}
}