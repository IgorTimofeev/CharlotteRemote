#include <cstdint>
#include <esp_timer.h>
#include "nvs.h"
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

		NVSSetup();
		ADCUnitsSetup();
		SPIBusSetup();

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
		_transceiver.setup();

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
			interpolationTick();

			// UI
			_application.tick();

			// Low priority tasks
			_transceiver.tick();
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

	void RC::interpolationTick() {
		const auto deltaTime = esp_timer_get_time() - _interpolationTickTime;

		if (deltaTime < constants::application::remoteDataInterpolationTickInterval)
			return;

		// 200 ms
		const float interpolationFactor = deltaTime / 200'000.f;

		_interpolationTickTime = esp_timer_get_time() + constants::application::remoteDataInterpolationTickInterval;

		_airspeedInterpolator.tick(interpolationFactor);
		_airspeedTrendInterpolator.tick(interpolationFactor);

		_altitudeInterpolator.tick(interpolationFactor);
		_altitudeTrendInterpolator.tick(interpolationFactor);

		_verticalSpeedInterpolator.tick(interpolationFactor);

		_pitchInterpolator.tick(interpolationFactor);
		_rollInterpolator.tick(interpolationFactor);
		_yawInterpolator.tick(interpolationFactor);
	}

	// ------------------------- Data -------------------------

	Interpolator &RC::getAirspeedInterpolator() {
		return _airspeedInterpolator;
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

	Interpolator &RC::getAirspeedTrendInterpolator() {
		return _airspeedTrendInterpolator;
	}

	Interpolator &RC::getAltitudeTrendInterpolator() {
		return _altitudeTrendInterpolator;
	}

	Interpolator &RC::getVerticalSpeedInterpolator() {
		return _verticalSpeedInterpolator;
	}

	uint16_t* RC::getThrottles() {
		return _throttles;
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

//		float pizda = yoba::toRadians(-90) + _joystickVertical.getMappedFloatValue() * yoba::toRadians(180);
//		_pitchInterpolator.setTargetValue(pizda);
//
//		pizda = yoba::toRadians(-90) + _joystickHorizontal.getMappedFloatValue() * yoba::toRadians(180);
//		_rollInterpolator.setTargetValue(pizda);
//
//		pizda = _ring.getMappedFloatValue() * yoba::toRadians(90);
//		_yawInterpolator.setTargetValue(pizda);

		_axisTickTime = esp_timer_get_time() + constants::axis::tickInterval;
	}

	void RC::NVSSetup() {
		auto status = nvs_flash_init();

		if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND) {
			// NVS partition was truncated and needs to be erased
			ESP_ERROR_CHECK(nvs_flash_erase());
			// Retry init
			ESP_ERROR_CHECK(nvs_flash_init());
		}
		else {
			ESP_ERROR_CHECK(status);
		}
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

	void RC::handleAircraftPacket(AircraftPacket* packet) {
		const auto time = esp_timer_get_time();
		const auto deltaTime = time - _aircraftPacketTime;
		_aircraftPacketTime = time;

		const auto oldSpeed = _airspeedInterpolator.getTargetValue();
		const auto oldAltitude = _altitudeInterpolator.getTargetValue();

		_pitchInterpolator.setTargetValue(packet->pitch);
		_rollInterpolator.setTargetValue(packet->roll);
		_yawInterpolator.setTargetValue(packet->yaw);

		_altitudeInterpolator.setTargetValue(packet->altitude);
		_airspeedInterpolator.setTargetValue(packet->speed);

		// Trends
		const auto deltaAltitude = _altitudeInterpolator.getTargetValue() - oldAltitude;

		// Airspeed & altitude, 10 sec
		_airspeedTrendInterpolator.setTargetValue((_airspeedInterpolator.getTargetValue() - oldSpeed) * 10'000'000 / deltaTime);
		_altitudeTrendInterpolator.setTargetValue(deltaAltitude * 10'000'000 / deltaTime);

		// Vertical speed, 1 min
		_verticalSpeedInterpolator.setTargetValue(deltaAltitude * 60'000'000 / deltaTime);
	}

	uint16_t RC::getAilerons() const {
		return _ailerons;
	}

	void RC::setAilerons(uint16_t ailerons) {
		_ailerons = ailerons;
	}

	uint16_t RC::getElevator() const {
		return _elevator;
	}

	void RC::setElevator(uint16_t elevator) {
		_elevator = elevator;
	}

	uint16_t RC::getRudder() const {
		return _rudder;
	}

	void RC::setRudder(uint16_t rudder) {
		_rudder = rudder;
	}

	uint16_t RC::getAileronsTrim() const {
		return _aileronsTrim;
	}

	void RC::setAileronsTrim(uint16_t aileronsTrim) {
		_aileronsTrim = aileronsTrim;
	}

	uint16_t RC::getElevatorTrim() const {
		return _elevatorTrim;
	}

	void RC::setElevatorTrim(uint16_t elevatorTrim) {
		_elevatorTrim = elevatorTrim;
	}

	uint16_t RC::getRudderTrim() const {
		return _rudderTrim;
	}

	void RC::setRudderTrim(uint16_t rudderTrim) {
		_rudderTrim = rudderTrim;
	}

	uint16_t RC::getFlaps() const {
		return _flaps;
	}

	void RC::setFlaps(uint16_t flaps) {
		_flaps = flaps;
	}

	uint16_t RC::getSpoilers() const {
		return _spoilers;
	}

	void RC::setSpoilers(uint16_t spoilers) {
		_spoilers = spoilers;
	}

	bool RC::isStrobeLights() const {
		return _strobeLights;
	}

	void RC::setStrobeLights(bool strobeLights) {
		_strobeLights = strobeLights;
	}

	float RC::getAltimeterPressure() const {
		return _altimeterPressure;
	}

	void RC::setAltimeterPressure(float altimeterPressure) {
		_altimeterPressure = altimeterPressure;
	}

	AltimeterMode RC::getAltimeterMode() const {
		return _altimeterMode;
	}

	void RC::setAltimeterMode(AltimeterMode altimeterMode) {
		_altimeterMode = altimeterMode;
	}

	float RC::getLatitude() const {
		return _latitude;
	}

	void RC::setLatitude(float latitude) {
		_latitude = latitude;
	}

	float RC::getLongitude() const {
		return _longitude;
	}

	void RC::setLongitude(float longitude) {
		_longitude = longitude;
	}
}