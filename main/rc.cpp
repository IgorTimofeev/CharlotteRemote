#include <cstdint>
#include <esp_timer.h>
#include "rc.h"
#include "constants.h"
#include "resources/sounds.h"
#include "ui/navigation/tabBar.h"
#include "ui/debugOverlay.h"

namespace pizda {
	using namespace yoba;

	RC& RC::getInstance() {
		static RC instance = RC();

		return instance;
	}

	[[noreturn]] void RC::run() {
		// -------------------------------- Main --------------------------------

		// Settings
		_settings.setup();

		// SPI bus
		SPIBusSetup();

		// Display
		_display.setup();

		// Renderer
		_renderer.setTarget(&_display);
		_application.setRenderer(&_renderer);

		// Touch panel
		_touchPanel.setup();
		_application.addInputDevice(&_touchPanel);

		// Transceiver
		gpio_config_t config = {
			.pin_bit_mask = BIT64(constants::hardware::transceiver::slaveSelect),
			.mode = GPIO_MODE_OUTPUT,
			.pull_up_en = GPIO_PULLUP_ENABLE,
			.pull_down_en = GPIO_PULLDOWN_DISABLE,
			.intr_type = GPIO_INTR_DISABLE
		};

		gpio_config(&config);
		gpio_set_level(constants::hardware::transceiver::slaveSelect, true);

//		_transceiver.setup();

		// Joysticks
//		_pitchHall.begin();
//		_rollHall.begin();

		// Slave
		_slave.setup();


		// Speaker
		_speaker.setup();


		// -------------------------------- UI --------------------------------

		// Theme
		Theme::setup(&_renderer);
		_application.setBackgroundColor(&Theme::bg1);

		// Tab bar
		_tabBar.setSelectedIndex(0);
		_application += &_tabBar;

		// Debug overlay
		updateDebugInfoVisibility();
		_application += &_debugOverlay;

		// -------------------------------- Main loop --------------------------------

		_speaker.play(resources::sounds::boot());

		while (true) {
			auto time = esp_timer_get_time();

			// High priority tasks
			_slave.tick();
			fromSlave();

			simulateFlightData();
			updateComputedData();

			// UI
			_application.tick();

			// Low priority tasks
			_speaker.tick();
			_settings.tick();

			_tickDeltaTime = (esp_timer_get_time() - time) / 1000;
		}
	}

	void RC::simulateFlightData() {
		const auto oldSpeed = _speedInterpolator.getTargetValue();
		const auto oldAltitude = _altitudeInterpolator.getTargetValue();

		// Test
		const auto testDeltaTime = (float) (esp_timer_get_time() - _simulationTickTime2);
		float testDelay = 1000000;

		if (testDeltaTime > testDelay) {
			// Throttle
			const auto handleFloat = [&](Interpolator& interpolator, float increment = 0.1f, float trigger = 0.05f) {
				if (std::abs(interpolator.getValue() - interpolator.getTargetValue()) > trigger)
					return;

				if (interpolator.getTargetValue() >= 1.0f) {
					interpolator.setTargetValue(0.0f);
				}
				else {
					interpolator.setTargetValue(std::min(interpolator.getTargetValue() + increment, 1.0f));
				}
			};

			handleFloat(_throttle1Interpolator, 0.2f, 0.01f);
			handleFloat(_throttle2Interpolator, 0.2f, 0.01f);

			// Speed
			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + (float) yoba::random(1, 20) / 10.0f * testDeltaTime / testDelay);
//			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + 2.0f);

			if (_speedInterpolator.getTargetValue() > 150)
				_speedInterpolator.setTargetValue(0);

			// Altitude
			_altitudeInterpolator.setTargetValue(_altitudeInterpolator.getTargetValue() + (float) yoba::random(1, 30) / 10.0f * testDeltaTime / testDelay);

			if (_altitudeInterpolator.getTargetValue() > 40)
				_altitudeInterpolator.setTargetValue(0);

//			// Pitch
//			_pitchInterpolator.setTargetValue(_pitchInterpolator.getTargetValue() + (float) toRadians(2));
//
//			if (toDegrees(_pitchInterpolator.getTargetValue()) > 10)
//				_pitchInterpolator.setTargetValue(toRadians(-10));
//
//			// Roll
//			_rollInterpolator.setTargetValue(_rollInterpolator.getTargetValue() + (float) toRadians(1));
//
//			if (toDegrees(_rollInterpolator.getTargetValue()) > 30)
//				_rollInterpolator.setTargetValue(toRadians(-30));

//			// Yaw
//			_yawInterpolator.setTargetValue(_yawInterpolator.getTargetValue() + (float) toRadians(10));
//
//			if (toDegrees(_yawInterpolator.getTargetValue()) > 170)
//				_yawInterpolator.setTargetValue(toRadians(-170));

			// A/P
			_simulationTickTime2 = esp_timer_get_time();

			const auto newSpeed = _speedInterpolator.getTargetValue();
			const auto newAltitude = _altitudeInterpolator.getTargetValue();

			const auto deltaSpeed = newSpeed - oldSpeed;
			const auto deltaAltitude = newAltitude - oldAltitude;

			// Shows where spd/alt should target in 10 sec
			const float trendValueDeltaTime = 10 * 1000000;
			const auto trendValueFactor = trendValueDeltaTime / testDeltaTime;

			_speedTrendInterpolator.setTargetValue(deltaSpeed * trendValueFactor);
			_altitudeTrendInterpolator.setTargetValue(deltaAltitude * trendValueFactor);
			_verticalSpeedInterpolator.setTargetValue(deltaAltitude * 60000.0f / testDeltaTime);

			// Controls
			handleFloat(_aileronsInterpolator);
			handleFloat(_flapsInterpolator);
			handleFloat(_rudderInterpolator);
			handleFloat(_elevatorInterpolator);
			handleFloat(_spoilersInterpolator);

			// Trim
			handleFloat(_aileronsTrimInterpolator);
			handleFloat(_rudderTrimInterpolator);
			handleFloat(_elevatorTrimInterpolator);
		}

		auto deltaTime = (float) (esp_timer_get_time() - _simulationTickTime1);

		if (deltaTime > constants::application::tickInterval) {
			const float interpolationFactor = deltaTime / testDelay;

			_throttle1Interpolator.tick(interpolationFactor);
			_throttle2Interpolator.tick(interpolationFactor);

			_speedInterpolator.tick(interpolationFactor);
			_speedTrendInterpolator.tick(interpolationFactor);

			_altitudeInterpolator.tick(interpolationFactor);
			_altitudeTrendInterpolator.tick(interpolationFactor);

			_verticalSpeedInterpolator.tick(interpolationFactor);

			_pitchInterpolator.tick(interpolationFactor);
			_rollInterpolator.tick(interpolationFactor);
			_yawInterpolator.tick(interpolationFactor);

			_aileronsInterpolator.tick(interpolationFactor);
			_rudderInterpolator.tick(interpolationFactor);
			_elevatorInterpolator.tick(interpolationFactor);
			_flapsInterpolator.tick(interpolationFactor);
			_spoilersInterpolator.tick(interpolationFactor);

			_aileronsTrimInterpolator.tick(interpolationFactor);
			_elevatorTrimInterpolator.tick(interpolationFactor);
			_rudderTrimInterpolator.tick(interpolationFactor);

			_application.invalidate();

			_simulationTickTime1 = esp_timer_get_time();
		}
	}

	// ------------------------- Data -------------------------

	LocalData &RC::getLocalData() {
		return _localData;
	}

	RemoteData &RC::getRemoteData() {
		return _remoteData;
	}

	ComputedData& RC::getComputedData() {
		return _computedData;
	}

	Interpolator& RC::getThrottle1Interpolator() {
		return _throttle1Interpolator;
	}

	Interpolator& RC::getThrottle2Interpolator() {
		return _throttle2Interpolator;
	}

	Interpolator &RC::getSpeedInterpolator() {
		return _speedInterpolator;
	}

	Interpolator &RC::getAltitudeInterpolator() {
		return _altitudeInterpolator;
	}

	Interpolator &RC::getPitchInterpolator() {
		return _pitchInterpolator;
	}

	Interpolator &RC::getRollInterpolator() {
		return _rollInterpolator;
	}

	Interpolator &RC::getYawInterpolator() {
		return _yawInterpolator;
	}

	Interpolator &RC::getSpeedTrendInterpolator() {
		return _speedTrendInterpolator;
	}

	Interpolator &RC::getAltitudeTrendInterpolator() {
		return _altitudeTrendInterpolator;
	}

	Interpolator &RC::getVerticalSpeedInterpolator() {
		return _verticalSpeedInterpolator;
	}

	Interpolator& RC::getAileronsInterpolator() {
		return _aileronsInterpolator;
	}

	Interpolator& RC::getFlapsInterpolator() {
		return _flapsInterpolator;
	}

	Interpolator& RC::getSpoilersInterpolator() {
		return _spoilersInterpolator;
	}

	Interpolator& RC::getRudderInterpolator() {
		return _rudderInterpolator;
	}

	Interpolator& RC::getElevatorInterpolator() {
		return _elevatorInterpolator;
	}

	Interpolator& RC::getAileronsTrimInterpolator() {
		return _aileronsTrimInterpolator;
	}

	Interpolator& RC::getElevatorTrimInterpolator() {
		return _elevatorTrimInterpolator;
	}

	Interpolator& RC::getRudderTrimInterpolator() {
		return _rudderTrimInterpolator;
	}

	Application& RC::getApplication() {
		return _application;
	}

	uint32_t RC::getTickDeltaTime() const {
		return _tickDeltaTime;
	}

	Settings& RC::getSettings() {
		return _settings;
	}

	void RC::updateDebugInfoVisibility() {
		_debugOverlay.setVisible(_settings.debugInfoVisible);
	}

	Speaker& RC::getSpeaker() {
		return _speaker;
	}

	Slave& RC::getSlave() {
		return _slave;
	}

	void RC::updateComputedData() {
		_computedData.getLatitudeSinAndCos().fromAngle(_remoteData.getLatitude());
		_computedData.getLongitudeSinAndCos().fromAngle(_remoteData.getLongitude());
		_computedData.getRollSinAndCos().fromAngle(_remoteData.getRoll());
		_computedData.getPitchSinAndCos().fromAngle(_remoteData.getPitch());
		_computedData.getYawSinAndCos().fromAngle(_remoteData.getYaw());
	}

	void RC::SPIBusSetup() {
		// SPI bus
		spi_bus_config_t busConfig {};
		busConfig.mosi_io_num = constants::hardware::spi::mosi;
		busConfig.miso_io_num = constants::hardware::spi::miso;
		busConfig.sclk_io_num = constants::hardware::spi::sck;
		busConfig.quadwp_io_num = -1;
		busConfig.quadhd_io_num = -1;
		busConfig.max_transfer_sz = 0xFFFF;

		ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO));
	}

	void RC::fromSlave() {
		float pizda = yoba::toRadians(-45) + (float) _slave.getData().joystickVertical / 4096.f * yoba::toRadians(90);
		_pitchInterpolator.setTargetValue(pizda);

		pizda = yoba::toRadians(-45) + (float) _slave.getData().joystickHorizontal / 4096.f * yoba::toRadians(90);
		_rollInterpolator.setTargetValue(pizda);

		pizda = (float) _slave.getData().ring / 4096.f * yoba::toRadians(45);
		_yawInterpolator.setTargetValue(pizda);
	}
}