#include <cstdint>
#include <esp_timer.h>
#include "nvs.h"
#include "rc.h"
#include "constants.h"
#include "resources/sounds.h"
#include "UI/navigation/routes.h"

namespace pizda {
	using namespace YOBA;

	RC& RC::getInstance() {
		static RC instance = RC();

		return instance;
	}

	[[noreturn]] void RC::run() {
		SPIBusSetup();
		ADCUnitsSetup();
		NVSSetup();

		// Settings
		_settings.setup();

		// Display
		_display.setup();

		// Renderer
		_renderer.setTarget(&_display);

		// Application
		_application.setRenderer(&_renderer);

		// Touch panel
		_touchPanel.setup();
		_application.addInputDevice(&_touchPanel);

//		// Transceiver
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

		Theme::setup(&_renderer);
		_application.setBackgroundColor(&Theme::bg1);
		_application += &_pageLayout;
		setRoute(&Routes::MFD);

		// -------------------------------- Take off --------------------------------

		_speaker.play(resources::Sounds::boot());

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
		// factorPerTick -> deltaTime us
		//
		// factorPerTick = factorPerSecond * deltaTime / 1'000'000

		// Roll / pitch / yaw / slip & skid, faster
		float interpolationFactor = 10.0f * (float) deltaTime / 1'000'000.f;
		_pitchInterpolator.tick(interpolationFactor);
		_rollInterpolator.tick(interpolationFactor);
		_yawInterpolator.tick(interpolationFactor);

		_slipAndSkidInterpolator.tick(interpolationFactor);

		_flightPathVectorPitchInterpolator.tick(interpolationFactor);
		_flightPathVectorYawInterpolator.tick(interpolationFactor);

		_flightDirectorPitchInterpolator.tick(interpolationFactor);
		_flightDirectorYawInterpolator.tick(interpolationFactor);

		// Airspeed / altitude, normal
		interpolationFactor = 8.0f * (float) deltaTime / 1'000'000.f;
		_airSpeedInterpolator.tick(interpolationFactor);
		_altitudeInterpolator.tick(interpolationFactor);
		_windDirectionInterpolator.tick(interpolationFactor);

		// Trends, slower
		interpolationFactor = 1.0f * (float) deltaTime / 1'000'000.f;
		_airSpeedTrendInterpolator.tick(interpolationFactor);
		_altitudeTrendInterpolator.tick(interpolationFactor);
		_verticalSpeedInterpolator.tick(interpolationFactor);

		_interpolationTickTime = esp_timer_get_time() + constants::application::interpolationTickInterval;
	}

	// ------------------------- Data -------------------------

	LowPassInterpolator& RC::getAirSpeedInterpolator() {
		return _airSpeedInterpolator;
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

	LowPassInterpolator& RC::getFlightPathVectorPitchInterpolator() {
		return _flightPathVectorPitchInterpolator;
	}

	LowPassInterpolator& RC::getFlightPathVectorYawInterpolator() {
		return _flightPathVectorYawInterpolator;
	}

	LowPassInterpolator& RC::getFlightDirectorPitchInterpolator() {
		return _flightDirectorPitchInterpolator;
	}

	LowPassInterpolator& RC::getFlightDirectorYawInterpolator() {
		return _flightDirectorYawInterpolator;
	}

	LowPassInterpolator& RC::getAirspeedTrendInterpolator() {
		return _airSpeedTrendInterpolator;
	}

	LowPassInterpolator& RC::getAltitudeTrendInterpolator() {
		return _altitudeTrendInterpolator;
	}

	LowPassInterpolator& RC::getVerticalSpeedInterpolator() {
		return _verticalSpeedInterpolator;
	}

	LowPassInterpolator& RC::getWindDirectionInterpolator() {
		return _windDirectionInterpolator;
	}

	float RC::getWindSpeed() const {
		return _windSpeed;
	}

	float RC::getGroundSpeed() const {
		return _groundSpeed;
	}

	uint8_t RC::getAircraftThrottle() const {
		return _aircraftThrottle;
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
//			ESP_LOGI("Encoder", "rotation: %ld", _encoder.getRotation());

			const auto rps = _encoder.getRotation() * (int32_t) 1'000'000 / (int32_t) deltaTime;
//			ESP_LOGI("Encoder", "rps: %ld", rps);

			auto event = EncoderRotateEvent(rps);
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
		busConfig.max_transfer_sz = 320 * 240 * 2;

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

		const auto oldSpeed = _airSpeedInterpolator.getTargetValue();
		const auto oldAltitude = _altitudeInterpolator.getTargetValue();

		// Direct
		_aircraftThrottle = packet->throttle;

		_geographicCoordinates.setLatitude(packet->latitudeRad);
		_geographicCoordinates.setLongitude(packet->longitudeRad);
		_geographicCoordinates.setAltitude(packet->altitudeM);

		_windSpeed = convertSpeed(packet->windSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);


		// Interpolators
		_altitudeInterpolator.setTargetValue(convertDistance(packet->altitudeM, DistanceUnit::meter, DistanceUnit::foot));
		_pitchInterpolator.setTargetValue(packet->pitchRad);
		_rollInterpolator.setTargetValue(packet->rollRad);
		_yawInterpolator.setTargetValue(packet->yawRad);

		_airSpeedInterpolator.setTargetValue(convertSpeed(packet->airSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot));
		_groundSpeed = convertSpeed(packet->groundSpeedMs, SpeedUnit::meterPerSecond, SpeedUnit::knot);

		_flightPathVectorPitchInterpolator.setTargetValue(packet->flightPathPitchRad);
		_flightPathVectorYawInterpolator.setTargetValue(packet->flightPathYawRad);

		_flightDirectorPitchInterpolator.setTargetValue(packet->flightDirectorPitchRad);
		_flightDirectorYawInterpolator.setTargetValue(packet->flightDirectorYawRad);

		_slipAndSkidInterpolator.setTargetValue(-1.f + (float) packet->slipAndSkid / (float) 0xFFFF * 2.f);

		_windDirectionInterpolator.setTargetValue(toRadians(packet->windDirectionDeg));

		// Trends
		const auto deltaAltitude = _altitudeInterpolator.getTargetValue() - oldAltitude;

		// Airspeed & altitude, 10 sec
		_airSpeedTrendInterpolator.setTargetValue((_airSpeedInterpolator.getTargetValue() - oldSpeed) * 10'000'000 / deltaTime);
		_altitudeTrendInterpolator.setTargetValue(deltaAltitude * 10'000'000 / deltaTime);

		// Vertical speed, 1 min
		_verticalSpeedInterpolator.setTargetValue(deltaAltitude * 60'000'000 / deltaTime);
	}

	const GeographicCoordinates& RC::getGeographicCoordinates() const {
		return _geographicCoordinates;
	}

	bool RC::isMenuVisible() {
		return _menu != nullptr;
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
		_pageLayout += _route->buildElement();
	}

	void RC::showMenu(const Route* route) {
		if (isMenuVisible())
			return;

		_menu = new Menu(route);
		_application += _menu;
	}

	void RC::hideMenu() {
		if (!isMenuVisible())
			return;

		_application -= _menu;
		delete _menu;
		_menu = nullptr;
	}
}