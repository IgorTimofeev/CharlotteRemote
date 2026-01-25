#include "axes.h"

#include <algorithm>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>

#include "rc.h"

namespace pizda {
	void Axes::setup() {
		auto& rc = RC::getInstance();
		
		_leverLeft.setup(
			rc.getAssignedADCOneshotUnit(config::axes::leverLeft::unit),
			config::axes::leverLeft::channel,
			config::axes::leverLeft::invertInput,
			&rc.getSettings().axes.leverLeft
		);

		_leverRight.setup(
			rc.getAssignedADCOneshotUnit(config::axes::leverRight::unit),
			config::axes::leverRight::channel,
			config::axes::leverRight::invertInput,
			&rc.getSettings().axes.leverRight
		);

		_joystickHorizontal.setup(
			rc.getAssignedADCOneshotUnit(config::axes::joystickHorizontal::unit),
			config::axes::joystickHorizontal::channel,
			config::axes::joystickHorizontal::invertInput,
			&rc.getSettings().axes.joystickHorizontal
		);

		_joystickVertical.setup(
			rc.getAssignedADCOneshotUnit(config::axes::joystickVertical::unit),
			config::axes::joystickVertical::channel,
			config::axes::joystickVertical::invertInput,
			&rc.getSettings().axes.joystickVertical
		);

		_ring.setup(
			rc.getAssignedADCOneshotUnit(config::axes::ring::unit),
			config::axes::ring::channel,
			config::axes::ring::invertInput,
			&rc.getSettings().axes.ring
		);
	}

	Axis& Axes::getLeverLeft() {
		return _leverLeft;
	}

	Axis& Axes::getLeverRight() {
		return _leverRight;
	}

	Axis& Axes::getJoystickHorizontal() {
		return _joystickHorizontal;
	}

	Axis& Axes::getJoystickVertical() {
		return _joystickVertical;
	}

	Axis& Axes::getRing() {
		return _ring;
	}

	void Axes::tick() {
		const auto time = esp_timer_get_time();

		if (time < _tickTime)
			return;

		_tickTime = time + 1'000'000 / config::axes::tickRateHz;

		_leverLeft.tick();
		_leverRight.tick();
		_joystickHorizontal.tick();
		_joystickVertical.tick();
		_ring.tick();
	}
}
