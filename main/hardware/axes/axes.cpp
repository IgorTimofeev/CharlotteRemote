#include "axes.h"

#include <algorithm>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

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

		xTaskCreate(
			[](void* arg) {
				static_cast<Axes*>(arg)->onStart();
			},
			"Battery",
			2 * 1024,
			this,
			1,
			nullptr
		);
	}

	[[noreturn]] void Axes::onStart() {
		while (true) {
			_leverLeft.read();
			_leverRight.read();
			_joystickHorizontal.read();
			_joystickVertical.read();
			_ring.read();

			vTaskDelay(pdMS_TO_TICKS(std::max<uint16_t>(1'000 / config::axis::tickRateHz, portTICK_PERIOD_MS)));
		}
	}
}
