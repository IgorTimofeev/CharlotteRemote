#include <cstdint>
#include <esp_timer.h>
#include "rc.h"
#include "constants.h"
#include "resources/sounds.h"

namespace pizda {
	using namespace yoba;

	RC& RC::getInstance() {
		static RC instance = RC();

		return instance;
	}

	[[noreturn]] void RC::run() {
		// -------------------------------- Main --------------------------------

		// GPIO
		SPIBusSetup();
		ADCUnitsSetup();

		// Settings
		_settings.setup();

		// Display
		_display.setup();

		// Renderer
		_renderer.setTarget(&_display);
		_application.setRenderer(&_renderer);

		// Touch panel
		_touchPanel.setup();
		_application.addInputDevice(&_touchPanel);

		// Transceiver
//		_transceiver.setup();

		// Axis
		_leverLeft.setup();
		_leverRight.setup();
		_joystickHorizontal.setup();
		_joystickVertical.setup();
		_ring.setup();

		// Encoder
		_encoder.setup();

		// Battery
		_battery.setup();

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
		updateDebugOverlayVisibility();
		_application += &_debugOverlay;

		// -------------------------------- Take off --------------------------------

		_speaker.play(resources::sounds::boot());

		while (true) {
			const auto time = esp_timer_get_time();

			// High priority tasks
			axisTick();
			encoderTick();
			simulateFlightData();
			updateComputedData();

			// UI
			_application.tick();

			// Low priority tasks
			_speaker.tick();
			_settings.tick();

			_tickDeltaTime = esp_timer_get_time() - time;

			// Skipping remaining tick time if any
			if (_tickDeltaTime < constants::application::mainTickInterval) {
				// FreeRTOS tasks can be only delayed by ms, so...
				vTaskDelay((constants::application::mainTickInterval - _tickDeltaTime) / 1000 / portTICK_PERIOD_MS);

//				ESP_LOGI("Main", "Skipping ticks for %lu ms", (constants::application::mainTickInterval - _tickDeltaTime) / 1000);
			}

			_tickDeltaTime /= 1000;
		}
	}

	void RC::simulateFlightData() {
		const auto oldSpeed = _speedInterpolator.getTargetValue();
		const auto oldAltitude = _altitudeInterpolator.getTargetValue();

		// Test
		auto deltaTime = (float) (esp_timer_get_time() - _simulationTickTime);
		float simulationTickInterval = 1000000;

		if (deltaTime > simulationTickInterval) {
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

			// Speed
			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + (float) yoba::random(1, 20) / 10.0f * deltaTime / simulationTickInterval);
//			_speedInterpolator.setTargetValue(_speedInterpolator.getTargetValue() + 2.0f);

			if (_speedInterpolator.getTargetValue() > 150)
				_speedInterpolator.setTargetValue(0);

			// Altitude
			_altitudeInterpolator.setTargetValue(_altitudeInterpolator.getTargetValue() + (float) yoba::random(1, 30) / 10.0f * deltaTime / simulationTickInterval);

			if (_altitudeInterpolator.getTargetValue() > 40)
				_altitudeInterpolator.setTargetValue(0);

			// A/P
			_simulationTickTime = esp_timer_get_time();

			const auto newSpeed = _speedInterpolator.getTargetValue();
			const auto newAltitude = _altitudeInterpolator.getTargetValue();

			const auto deltaSpeed = newSpeed - oldSpeed;
			const auto deltaAltitude = newAltitude - oldAltitude;

			// Shows where spd/alt should target in 10 sec
			const float trendValueDeltaTime = 10'000'000;
			const auto trendValueFactor = trendValueDeltaTime / deltaTime;

			_speedTrendInterpolator.setTargetValue(deltaSpeed * trendValueFactor);
			_altitudeTrendInterpolator.setTargetValue(deltaAltitude * trendValueFactor);
			_verticalSpeedInterpolator.setTargetValue(deltaAltitude * 60000000.0f / deltaTime);

			// Controls
			handleFloat(_aileronsInterpolator);
			handleFloat(_rudderInterpolator);
			handleFloat(_elevatorInterpolator);

			// Trim
			handleFloat(_aileronsTrimInterpolator);
			handleFloat(_rudderTrimInterpolator);
			handleFloat(_elevatorTrimInterpolator);
		}

		deltaTime = (float) (esp_timer_get_time() - _interpolatorTickTime);

		if (deltaTime > constants::application::remoteDataInterpolationTickInterval) {
			constexpr static float interpolationTickInterval = 500 * 1000;

			const float interpolationFactor = deltaTime / interpolationTickInterval;

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

			_aileronsTrimInterpolator.tick(interpolationFactor);
			_elevatorTrimInterpolator.tick(interpolationFactor);
			_rudderTrimInterpolator.tick(interpolationFactor);

			_interpolatorTickTime = esp_timer_get_time();
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

	void RC::updateDebugOverlayVisibility() {
		_debugOverlay.setVisible(_settings.debugInfoVisible);
	}

	Speaker& RC::getSpeaker() {
		return _speaker;
	}

	Axis& RC::getLeverLeft() {
		return _leverLeft;
	}

	Encoder& RC::getEncoder() {
		return _encoder;
	}

	Axis& RC::getLeverRight() {
		return _leverRight;
	}

	Axis& RC::getJoystickHorizontal() {
		return _joystickHorizontal;
	}

	Axis& RC::getJoystickVertical() {
		return _joystickVertical;
	}

	Axis& RC::getRing() {
		return _ring;
	}

	Battery& RC::getBattery() {
		return _battery;
	}

	void RC::updateComputedData() {
		_computedData.getLatitudeSinAndCos().fromAngle(_remoteData.getLatitude());
		_computedData.getLongitudeSinAndCos().fromAngle(_remoteData.getLongitude());
		_computedData.getRollSinAndCos().fromAngle(_remoteData.getRoll());
		_computedData.getPitchSinAndCos().fromAngle(_remoteData.getPitch());
		_computedData.getYawSinAndCos().fromAngle(_remoteData.getYaw());
	}

	void RC::encoderTick() {
		if (!_encoder.interrupted())
			return;

		_encoder.acknowledgeInterrupt();

		// Rotation
		if (std::abs(_encoder.getRotation()) > 2) {
			const uint32_t time = esp_timer_get_time();
			const uint32_t deltaTime = time - _encoderRotationTime;
			_encoderRotationTime = time;

			// No cast = sign lost
			auto event = EncoderRotateEvent(_encoder.getRotation() * 1'000'000 / (int32_t) deltaTime);
			_application.handleEvent(&event);

			_encoder.setRotation(0);
		}

		// Push
		if (_encoder.isPressed() != _encoderWasPressed) {
			auto event = EncoderPushEvent(_encoder.isPressed());
			_application.handleEvent(&event);

			_encoderWasPressed = _encoder.isPressed();
		}
	}

	void RC::axisTick() {
		if (esp_timer_get_time() < _axisTickTime)
			return;

		_leverLeft.tick();
		_leverRight.tick();
		_joystickHorizontal.tick();
		_joystickVertical.tick();
		_ring.tick();
		_battery.tick();

//		ESP_LOGI("HID", "------------------------------------");
//		ESP_LOGI("HID", "Lever left: %d", _leverLeft.getValue());
//		ESP_LOGI("HID", "Encoder rotation: %ld", _encoder.getRotation());
//		ESP_LOGI("HID", "Encoder pressed: %b", _encoder.isPressed());
//		ESP_LOGI("HID", "Lever right: %d", _leverRight.getValue());
//		ESP_LOGI("HID", "Joy horiz: %d", _joystickHorizontal.getValue());
//		ESP_LOGI("HID", "Hoy vert: %d", _joystickVertical.getValue());
//		ESP_LOGI("HID", "Ring: %d", _ring.getValue());
//		ESP_LOGI("HID", "BChr: %d", _battery.getCharge());

		float pizda = yoba::toRadians(-90) + _joystickVertical.getProcessedFloatValue() * yoba::toRadians(180);
		_pitchInterpolator.setTargetValue(pizda);

		pizda = yoba::toRadians(-90) + _joystickHorizontal.getProcessedFloatValue() * yoba::toRadians(180);
		_rollInterpolator.setTargetValue(pizda);

		pizda = _ring.getProcessedFloatValue() * yoba::toRadians(90);
		_yawInterpolator.setTargetValue(pizda);

		_axisTickTime = esp_timer_get_time() + constants::axis::tickInterval;
	}

	void RC::SPIBusSetup() {
		spi_bus_config_t busConfig {};
		busConfig.mosi_io_num = constants::spi::mosi;
		busConfig.miso_io_num = constants::spi::miso;
		busConfig.sclk_io_num = constants::spi::sck;
		busConfig.quadwp_io_num = -1;
		busConfig.quadhd_io_num = -1;
		busConfig.max_transfer_sz = 0xFFFF;

		ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &busConfig, SPI_DMA_CH_AUTO));
	}

	void RC::ADCUnitsSetup() {
		adc_oneshot_unit_init_cfg_t ADC1UnitConfig = {
			.unit_id = ADC_UNIT_1,
			.clk_src = ADC_RTC_CLK_SRC_DEFAULT,
			.ulp_mode = ADC_ULP_MODE_DISABLE
		};

		ESP_ERROR_CHECK(adc_oneshot_new_unit(&ADC1UnitConfig, &constants::adc::unit1));
	}
}