#include "instrumentsLayout.h"
#include "../../../../rc.h"

namespace pizda {
	const PFDLandingGearRetractedImage InstrumentsLayout::_landingGearRetractedImage = {};
	const PFDLandingGearExtendedImage InstrumentsLayout::_landingGearExtendedImage = {};

	InstrumentsLayout::InstrumentsLayout() {
		_backgroundRect.setFillColor(&Theme::bg1);
		*this += &_backgroundRect;

		// Titles
		_titlesRect.setFillColor(&Theme::bg2);
		_titlesRect.setHeight(12);
		_titlesRect.setVerticalAlignment(Alignment::start);
		*this += &_titlesRect;

		// Row
		_row.setOrientation(Orientation::horizontal);
		_row.setSpacing(15);
		_row.setHorizontalAlignment(Alignment::center);
		_row.setMargin(Margin(8, 1, 8, 0));

		// Menu button
		_row += &_menuButton;

		// Throttle
		Theme::apply(&_throttleTitle);

		const uint8_t contentHeight = InstrumentsLayout::panelSize - _row.getMargin().getTop() - _throttleTitle.getSpacing() - _throttleTitle.getTitle().getFont()->getHeight();

		_throttleRow.setOrientation(Orientation::horizontal);
		_throttleRow.setSpacing(10);

		_throttle1Indicator.setSize(Size(7, contentHeight));
		_throttleRow += &_throttle1Indicator;

		_throttle2Indicator.setSize(_throttle1Indicator.getSize());
		_throttleRow += &_throttle2Indicator;

		_row += &_throttleTitle;

		// Controls
		_controlsRows.setSpacing(5);
		_controlsRows += &_flapsAndSpoilersIndicator;
		_controlsRows += &_landingGearImageView;

		Theme::apply(&_controlsTitle);
		_row += &_controlsTitle;

		// Elevator trim
		Theme::apply(&_elevatorTrimTitle);
		_elevatorTrimIndicator.setSize(Size(5, contentHeight - 4));
		_elevatorTrimIndicator.setSuggestedMinimum(0xFFFF * 40 / 100);
		_elevatorTrimIndicator.setSuggestedMaximum(0xFFFF * 60 / 100);
		_row += &_elevatorTrimTitle;

		// Battery
		Theme::apply(&_batteryIndicatorTitle);

		_batteryIndicatorController.setSize(Size(32, 12));
		_batteryIndicatorAircraft.setSize(_batteryIndicatorController.getSize());

		_batteryIndicatorRows.setSpacing(5);
		_batteryIndicatorRows += &_batteryIndicatorController;
		_batteryIndicatorRows += &_batteryIndicatorAircraft;

		_row += &_batteryIndicatorTitle;

		*this += &_row;
	}

	void InstrumentsLayout::onTick() {
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

	void InstrumentsLayout::onEvent(Event* event) {
		Layout::onEvent(event);

		if (event->getTypeID() == EncoderRotateEvent::typeID) {
			auto rotateEvent = (EncoderRotateEvent*) event;

			ESP_LOGI("Encoder", "RPS: %ld", rotateEvent->getRPS());

			auto& settings = RC::getInstance().getSettings();

			// Throttle
			settings.controls.throttles[0] = yoba::addSaturating(settings.controls.throttles[0], rotateEvent->getRPSFactor(60, 1, 10) * 0xFFFF / 100);
			settings.controls.throttles[1] = settings.controls.throttles[0];

			event->setHandled(true);
		}
		else if (event->getTypeID() == EncoderPushEvent::typeID) {
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