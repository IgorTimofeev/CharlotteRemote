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

		// Page layout
		_application += &_pageLayout;

		setRoute(&Routes::PFD);

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

		if (deltaTime < constants::application::interpolationTickInterval)
			return;

		// Principle of calculating the interpolation factor:
		//
		// factorPerSecond -> 1'000'000 us
		// factorPerTick -> tickInterval us
		//
		// factorPerTick = factorPerSecond * tickInterval / 1'000'000

		// Roll / pitch / yaw / slip&skid, faster
		float interpolationFactor = 10.0f * (float) constants::application::interpolationTickInterval / 1'000'000.f;
		_pitchInterpolator.tick(interpolationFactor);
		_rollInterpolator.tick(interpolationFactor);
		_yawInterpolator.tick(interpolationFactor);
		_slipAndSkidInterpolator.tick(interpolationFactor);

		// Airspeed / altitude, normal
		interpolationFactor = 8.0f * (float) constants::application::interpolationTickInterval / 1'000'000.f;
		_airspeedInterpolator.tick(interpolationFactor);
		_altitudeInterpolator.tick(interpolationFactor);

		// Trends, slower
		interpolationFactor = 1.0f * (float) constants::application::interpolationTickInterval / 1'000'000.f;
		_airspeedTrendInterpolator.tick(interpolationFactor);
		_altitudeTrendInterpolator.tick(interpolationFactor);
		_verticalSpeedInterpolator.tick(interpolationFactor);

		_interpolationTickTime = esp_timer_get_time() + constants::application::interpolationTickInterval;
	}

	// ------------------------- Data -------------------------

	LowPassInterpolator& RC::getAirspeedInterpolator() {
		return _airspeedInterpolator;
	}

	LowPassInterpolator& RC::getAltitudeInterpolator() {
		return _altitudeInterpolator;
	}

	LowPassInterpolator& RC::getPitchInterpolator() {
		return _pitchInterpolator;
	}

	LowPassInterpolator& RC::getRollInterpolator() {
		return _rollInterpolator;
	}

	LowPassInterpolator& RC::getYawInterpolator() {
		return _yawInterpolator;
	}

	LowPassInterpolator& RC::getSlipAndSkidInterpolator() {
		return _slipAndSkidInterpolator;
	}

	LowPassInterpolator& RC::getAirspeedTrendInterpolator() {
		return _airspeedTrendInterpolator;
	}

	LowPassInterpolator& RC::getAltitudeTrendInterpolator() {
		return _altitudeTrendInterpolator;
	}

	LowPassInterpolator& RC::getVerticalSpeedInterpolator() {
		return _verticalSpeedInterpolator;
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

		_geocentricCoordinates.setLatitude(packet->latitude);
		_geocentricCoordinates.setLongitude(packet->longitude);
		_geocentricCoordinates.setAltitude(packet->altitude);

		_cartesianCoordinates = _geocentricCoordinates.toCartesian();

		if (time > _flightPathVectorTime) {
			ESP_LOGI("FPA", "-------------------------");

			ESP_LOGI("FPA", "PYR: %f, %f, %f", toDegrees(packet->pitch), toDegrees(packet->yaw), toDegrees(packet->roll));

			auto delta = _cartesianCoordinates - _flightPathVectorCartesianCoordinates;
			_flightPathVectorCartesianCoordinates = _cartesianCoordinates;

			ESP_LOGI("FPA", "Delta: %f, %f, %f", delta.getX(), delta.getY(), delta.getZ());

			// Transforming earth-based coordinate system to aircraft-based
			delta = delta.rotateAroundVerticalAxis(-packet->longitude);
			delta = delta.rotateAroundHorizontalAxis(-packet->latitude);

			ESP_LOGI("FPA", "Rotated: %f, %f, %f", delta.getX(), delta.getY(), delta.getZ());

			_flightPathVector = delta;

			const auto length = _flightPathVector.getLength();

			_flightPathAngles = Vector2F(
				std::asinf(_flightPathVector.getZ() / length),
				std::atan2f(_flightPathVector.getY(), _flightPathVector.getX())
			);

			ESP_LOGI("FPA", "Angles: %f, %f", toDegrees(_flightPathAngles.getX()), toDegrees(_flightPathAngles.getY()));

			_flightPathVectorTime = time + _flightPathVectorInterval;
		}

		_pitchInterpolator.setTargetValue(packet->pitch);
		_rollInterpolator.setTargetValue(packet->roll);
		_yawInterpolator.setTargetValue(packet->yaw);
		_slipAndSkidInterpolator.setTargetValue(packet->slipAndSkid);
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

	float RC::getAltimeterPressure() const {
		return _altimeterPressure;
	}

	void RC::setAltimeterPressure(float altimeterPressure) {
		_altimeterPressure = altimeterPressure;
	}

	const GeocentricCoordinates& RC::getGeocentricCoordinates() const {
		return _geocentricCoordinates;
	}

	bool RC::isMenuVisible() {
		return _menu != nullptr;
	}

	void RC::setMenuVisibility(bool state) {
		if (state) {
			if (isMenuVisible())
				return;

			_menu = new Menu();
			_application += _menu;
		}
		else {
			if (!isMenuVisible())
				return;

			_application -= _menu;
			delete _menu;
			_menu = nullptr;
		}
	}

	bool RC::isDebugOverlayVisible() {
		return _debugOverlay != nullptr;
	}

	void RC::setDebugOverlayVisibility(bool state) {
		if (state) {
			if (isDebugOverlayVisible())
				return;

			_debugOverlay = new DebugOverlay();
			_application += _debugOverlay;
		}
		else {
			if (!isDebugOverlayVisible())
				return;

			_application -= _debugOverlay;
			delete _debugOverlay;
			_debugOverlay = nullptr;
		}
	}

	const Route* RC::getRoute() {
		return _route;
	}

	void RC::setRoute(const Route* route) {
		_route = route;

		// Removing old page
		if (_pageLayout.getChildrenCount() > 0) {
			auto oldPage = _pageLayout[0];

			_pageLayout.removeChildAt(0);

			delete oldPage;
		}

		// Adding new page
		_pageLayout += _route->newPage();
	}

	const Vector3F& RC::getFlightPathVector() const {
		return _flightPathVector;
	}

	const Vector2F& RC::getFlightPathAngles() const {
		return _flightPathAngles;
	}
}