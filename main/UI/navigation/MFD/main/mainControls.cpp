#include "mainControls.h"
#include "../../../../rc.h"
#include "../../../../resources/images.h"

namespace pizda {
	MainControls::MainControls() {
		const uint16_t titleHeight = Theme::fontSmall.getHeight() + titleVerticalOffset * 2;
		const uint16_t contentHeight = panelSize - titleHeight - contentVerticalOffset * 2;

		setHeight(panelSize);

		row.setSpacing(15);

		// Throttle
		_throttleRow.setOrientation(Orientation::horizontal);
		_throttleRow.setSpacing(10);

		_throttle1Indicator.setSize(Size(7, contentHeight + 2));
		_throttleRow += &_throttle1Indicator;

		_throttle2Indicator.setSize(_throttle1Indicator.getSize());
		_throttleRow += &_throttle2Indicator;

		applyTitlerStyle(_throttleTitle);
		row += &_throttleTitle;

		// Controls
		_controlsRows.setSpacing(5);
		_controlsRows += &_flapsAndSpoilersIndicator;
		_controlsRows += &_landingGearImageView;

		applyTitlerStyle(_controlsTitle);
		row += &_controlsTitle;

		// Elevator trim
		_elevatorTrimIndicator.setSize(Size(5, contentHeight));
		_elevatorTrimIndicator.setSuggestedMinimum(0xFFFF * 40 / 100);
		_elevatorTrimIndicator.setSuggestedMaximum(0xFFFF * 60 / 100);

		applyTitlerStyle(_elevatorTrimTitle);
		row += &_elevatorTrimTitle;

		// Battery
		_batteryIndicatorController.setSize(Size(32, 12));
		_batteryIndicatorAircraft.setSize(_batteryIndicatorController.getSize());

		_batteryIndicatorRows.setSpacing(5);
		_batteryIndicatorRows += &_batteryIndicatorController;
		_batteryIndicatorRows += &_batteryIndicatorAircraft;

		applyTitlerStyle(_batteryIndicatorTitle);
		row += &_batteryIndicatorTitle;
	}

	void MainControls::onTick() {
		Layout::onTick();

		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// Throttles
		_throttle1Indicator.setRemoteValue(settings.controls.throttle);
		_throttle1Indicator.setAircraftValue(rc.getAircraftThrottle());

		_throttle2Indicator.setRemoteValue(_throttle1Indicator.getRemoteValue());
		_throttle2Indicator.setAircraftValue(_throttle1Indicator.getAircraftValue());

		// Controls
		_landingGearImageView.setImage(
			settings.controls.landingGear
			? reinterpret_cast<const Image*>(&resources::Images::MFDLandingGearExtended)
			: reinterpret_cast<const Image*>(&resources::Images::MFDLandingGearRetracted)
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
				settings.controls.throttle = YOBA::addSaturating(settings.controls.throttle, rotateEvent->getRPSFactor(60, 1, 10) * 0xFF / 100);

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

}