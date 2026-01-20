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
			rc.getAssignedADCOneshotUnit(config::axis::leverLeft::unit),
			config::axis::leverLeft::channel,
			config::axis::leverLeft::invertInput,
			&rc.getSettings().axis.leverLeft
		);

		_leverRight.setup(
			rc.getAssignedADCOneshotUnit(config::axis::leverRight::unit),
			config::axis::leverRight::channel,
			config::axis::leverRight::invertInput,
			&rc.getSettings().axis.leverRight
		);

		_joystickHorizontal.setup(
			rc.getAssignedADCOneshotUnit(config::axis::joystickHorizontal::unit),
			config::axis::joystickHorizontal::channel,
			config::axis::joystickHorizontal::invertInput,
			&rc.getSettings().axis.joystickHorizontal
		);

		_joystickVertical.setup(
			rc.getAssignedADCOneshotUnit(config::axis::joystickVertical::unit),
			config::axis::joystickVertical::channel,
			config::axis::joystickVertical::invertInput,
			&rc.getSettings().axis.joystickVertical
		);

		_ring.setup(
			rc.getAssignedADCOneshotUnit(config::axis::ring::unit),
			config::axis::ring::channel,
			config::axis::ring::invertInput,
			&rc.getSettings().axis.ring
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
		if (esp_timer_get_time() < _tickTime)
			return;

		_leverLeft.tick();
		_leverRight.tick();
		_joystickHorizontal.tick();
		_joystickVertical.tick();
		_ring.tick();

		_tickTime = esp_timer_get_time() + 1'000'000 / config::axis::tickRateHz;
	}
}
