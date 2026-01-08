#include <cstdint>
#include <esp_timer.h>
#include "nvs.h"
#include "rc.h"
#include "config.h"
#include "resources/sounds.h"
#include "resources/images.h"
#include "UI/navigation/routes.h"

namespace pizda {
	using namespace YOBA;

	RC& RC::getInstance() {
		static auto instance = RC();

		return instance;
	}

	[[noreturn]] void RC::run() {
		// First, let's render a splash screen while we wait for the peripherals to finish warming up
		SPIBusSetup();
		GPIOSetup();
		
		// After applying power or a hard reset, the LCD panel will be turned off, its internal pixel
		// buffer will contain random garbage, and the driver will wait for pixel data to be received
		//
		// If you turn on the LCD before sending pixels, this garbage will be immediately shown,
		// which will definitely cause bleed from videophiles eyes
		//
		// So...
		_display.setup();
		_renderer.setTarget(&_display);

		// Rendering splash screen
		Theme::setup(&_renderer);
		_renderer.clear(&Theme::bg1);
		_renderer.renderImage(Point(), &resources::images::splashScreen);
		_renderer.flush();

		// Turning display on
		_display.turnOn();

		// NVS is required by settings & Wi-Fi
		NVSSetup();
		ADCUnitsSetup();

		// Settings contain calibration data for the ADC axes, so they should be read first
		_settings.readAll();

		// Peripherals
		_touchPanel.setup();

		_encoder.setup();
		_encoder.setMinimumDelta(4);

		_battery.setup();
		_audioPlayer.setup();

		_leverLeft.setup();
		_leverRight.setup();
		_joystickHorizontal.setup();
		_joystickVertical.setup();
		_ring.setup();

		// Transceiver
		if (!_transceiver.setup())
			startErrorLoop("failed to setup XCVR");
		
		_packetHandler.setTransceiver(&_transceiver);
		_packetHandler.start();
		
		// Application
		_application.setRenderer(&_renderer);
		_application.addHID(&_touchPanel);
		_application.addHID(&_encoder);

		// UI
		_application.setBackgroundColor(&Theme::bg1);
		_application += &_pageLayout;
		_application += &_openMenuButton;

		setRoute(&Routes::MFD);
		updateDebugOverlayVisibility();

		// This shit is blazingly 🔥 fast 🚀, so letting user enjoy logo for a few moments
		vTaskDelay(pdMS_TO_TICKS(1000));

		// Beep beep
		_audioPlayer.play(resources::sounds::boot);

		// Main loop
		while (true) {
			const auto tickStartTime = esp_timer_get_time();

			// High priority tasks
			axisTick();
			interpolationTick();

			// UI
			_application.tick();
			_application.render();

 			_tickDeltaTime = esp_timer_get_time() - tickStartTime;

 			// Skipping remaining tick time if any
 			if (_tickDeltaTime < config::application::UITickIntervalUs) {
				// FreeRTOS tasks can be only delayed by ms, so...
				const auto delayMs = (config::application::UITickIntervalUs - _tickDeltaTime) / 1000;
				
				if (delayMs >= portTICK_PERIOD_MS) {
					vTaskDelay(pdMS_TO_TICKS(delayMs));
				}
				// Meh
				else {
					taskYIELD();
				}
 			}
		}
	}
	
	float RC::applyLPF(float oldValue, float newValue, float factor) {
		return
			_settings.personalization.LPF
			? LowPassFilter::apply(oldValue, newValue, factor)
			: newValue;
	}
	
	float RC::applyLPFForAngleRad(float oldValue, float newValue, float factor) {
		return
			_settings.personalization.LPF
			? LowPassFilter::applyForAngleRad(oldValue, newValue, factor)
			: newValue;
	}
	
	void RC::interpolationTick() {
		const auto deltaTimeUs = esp_timer_get_time() - _interpolationTickTime;

		if (deltaTimeUs < config::application::dataInterpolationTickIntervalUs)
			return;

		// Principle of calculating the interpolation factor:
		//
		// factorPerSecond -> 1'000'000 us
		// factorPerTick -> deltaTime us
		//
		// factorPerTick = factorPerSecond * deltaTime / 1'000'000

		// Fast
		float LPFFactor = LowPassFilter::getFactor(5.0f, deltaTimeUs);
		
		// Pitch
		_aircraftData.computed.pitchRad = applyLPFForAngleRad(
			_aircraftData.computed.pitchRad,
			_aircraftData.raw.pitchRad,
			LPFFactor
		);
		
		// Roll
		_aircraftData.computed.rollRad = applyLPFForAngleRad(
			_aircraftData.computed.rollRad,
			_aircraftData.raw.rollRad,
			LPFFactor
		);
		
		// Yaw
		_aircraftData.computed.yawRad = applyLPFForAngleRad(
			_aircraftData.computed.yawRad,
			_aircraftData.raw.yawRad,
			LPFFactor
		);
		
		// Heading
		_aircraftData.computed.headingDeg = normalizeAngleDeg360(toDegrees(-_aircraftData.computed.yawRad));
//		ESP_LOGI("PIZDA", "raw: %f, raw deg: %f, computed: %f, deg: %f, heading: %f", _aircraftData.raw.yawRad, toDegrees(-_aircraftData.raw.yawRad), _aircraftData.computed.yawRad, toDegrees(_aircraftData.computed.yawRad), _aircraftData.computed.headingDeg);

		// Slip & skid
		_aircraftData.computed.slipAndSkidFactor = applyLPF(
			_aircraftData.computed.slipAndSkidFactor,
			_aircraftData.raw.slipAndSkidFactor,
			LPFFactor
		);

		// Flight path vector
		_aircraftData.computed.flightPathVectorPitchRad = applyLPF(
			_aircraftData.computed.flightPathVectorPitchRad,
			_aircraftData.raw.flightPathVectorPitchRad,
			LPFFactor
		);
		
		_aircraftData.computed.flightPathVectorYawRad = applyLPF(
			_aircraftData.computed.flightPathVectorYawRad,
			_aircraftData.raw.flightPathVectorYawRad,
			LPFFactor
		);
		
		// Flight director
		_aircraftData.computed.autopilot.rollRad = applyLPF(
			_aircraftData.computed.autopilot.rollRad,
			_aircraftData.raw.autopilot.rollRad,
			LPFFactor
		);
		
		_aircraftData.computed.autopilot.pitchRad = applyLPF(
			_aircraftData.computed.autopilot.pitchRad,
			_aircraftData.raw.autopilot.pitchRad,
			LPFFactor
		);
		
		// Normal
		LPFFactor = LowPassFilter::getFactor(3.0f, deltaTimeUs);
		
		// Air speed
		_aircraftData.computed.airspeedKt = applyLPF(
			_aircraftData.computed.airspeedKt,
			Units::convertSpeed(_aircraftData.raw.airspeedMPS, SpeedUnit::meterPerSecond, SpeedUnit::knot),
			LPFFactor
		);
		
		// Altitude
		_aircraftData.computed.altitudeFt = applyLPF(
			_aircraftData.computed.altitudeFt,
			Units::convertDistance(_aircraftData.raw.coordinates.getAltitude(), DistanceUnit::meter, DistanceUnit::foot),
			LPFFactor
		);
		
		// Wind direction
		_aircraftData.computed.windDirectionRad = applyLPF(
			_aircraftData.computed.windDirectionRad,
			_aircraftData.raw.windDirectionRad,
			LPFFactor
		);
		
		// Throttle
		_aircraftData.computed.throttle_0_1 = applyLPF(
			_aircraftData.computed.throttle_0_1,
			static_cast<float>(_aircraftData.raw.throttle_0_255) / 255.f,
			LPFFactor
		);
		
		// Slower
		LPFFactor = LowPassFilter::getFactor(1.0f, deltaTimeUs);
		
		// Air speed trend
		_aircraftData.computed.airspeedTrendKt = applyLPF(
			_aircraftData.computed.airspeedTrendKt,
			Units::convertSpeed(_aircraftData.raw.airspeedTrendMPS, SpeedUnit::meterPerSecond, SpeedUnit::knot),
			LPFFactor
		);
		
		// Altitude trend
		_aircraftData.computed.altitudeTrendFt = applyLPF(
			_aircraftData.computed.altitudeTrendFt,
			Units::convertDistance(_aircraftData.raw.altitudeTrendM, DistanceUnit::meter, DistanceUnit::foot),
			LPFFactor
		);
		
		// Vertical speed
		_aircraftData.computed.verticalSpeedFtPM = applyLPF(
			_aircraftData.computed.verticalSpeedFtPM,
			Units::convertDistance(_aircraftData.raw.verticalSpeedMPM, DistanceUnit::meter, DistanceUnit::foot),
			LPFFactor
		);
		
		// Smooth as fuck
		LPFFactor = LowPassFilter::getFactor(0.5f, deltaTimeUs);
		
		_interpolationTickTime = esp_timer_get_time();
	}

	// ------------------------- Data -------------------------

	Application& RC::getApplication() {
		return _application;
	}
	
	RemoteData& RC::getRemoteData() {
		return _remoteData;
	}
	
	AircraftData& RC::getAircraftData() {
		return _aircraftData;
	}

	NavigationData& RC::getNavigationData() {
		return _navigationData;
	}
	
	uint32_t RC::getTickDeltaTime() const {
		return _tickDeltaTime;
	}
	
	Settings& RC::getSettings() {
		return _settings;
	}
	
	AudioPlayer& RC::getAudioPlayer() {
		return _audioPlayer;
	}

	Axis& RC::getLeverLeft() {
		return _leverLeft;
	}

	PushButtonEncoder& RC::getEncoder() {
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

	void RC::axisTick() {
		if (esp_timer_get_time() < _axisTickTimeUs)
			return;

		_leverLeft.tick();
		_leverRight.tick();
		_joystickHorizontal.tick();
		_joystickVertical.tick();
		_ring.tick();
		_battery.tick();
		
		_axisTickTimeUs = esp_timer_get_time() + config::axis::tickIntervalUs;
	}

	void RC::NVSSetup() {
		const auto status = nvs_flash_init();

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

	void RC::SPIBusSetup() const {
		spi_bus_config_t config {};
		config.mosi_io_num = config::spi::MOSI;
		config.miso_io_num = config::spi::MISO;
		config.sclk_io_num = config::spi::SCK;
		config.quadwp_io_num = -1;
		config.quadhd_io_num = -1;
		config.max_transfer_sz = _display.getSize().getSquare() * 2;

		ESP_ERROR_CHECK(spi_bus_initialize(config::spi::device, &config, SPI_DMA_CH_AUTO));
	}
	
	void RC::GPIOSetup() const {
		// Slave selects
		gpio_config_t g = {};
		g.pin_bit_mask = (1ULL << config::screen::SS) | (1ULL << config::transceiver::SS);
		g.mode = GPIO_MODE_OUTPUT;
		g.pull_up_en = GPIO_PULLUP_DISABLE;
		g.pull_down_en = GPIO_PULLDOWN_DISABLE;
		g.intr_type = GPIO_INTR_DISABLE;
		gpio_config(&g);
		
		gpio_set_level(config::screen::SS, true);
		gpio_set_level(config::transceiver::SS, true);
	}

	void RC::ADCUnitsSetup() {
		constexpr adc_oneshot_unit_init_cfg_t ADC1UnitConfig = {
			.unit_id = config::adc::unit,
			.clk_src = ADC_RTC_CLK_SRC_DEFAULT,
			.ulp_mode = ADC_ULP_MODE_DISABLE
		};

		ESP_ERROR_CHECK(adc_oneshot_new_unit(&ADC1UnitConfig, &config::adc::oneshotUnit));
	}

	void RC::updateDebugOverlayVisibility() {
		if (_settings.personalization.debugOverlay) {
			if (_debugOverlay != nullptr)
				return;

			_debugOverlay = new DebugOverlay();
			_application += _debugOverlay;
		}
		else {
			if (_debugOverlay == nullptr)
				return;

			_application -= _debugOverlay;
			delete _debugOverlay;
			_debugOverlay = nullptr;
		}
	}

	const Route* RC::getRoute() const {
		return _route;
	}

	void RC::setRoute(const Route* route) {
		if (route == _route)
			return;

		_route = route;

		// Removing old page
		if (_pageLayout.getChildrenCount() > 0) {
			const auto oldPage = _pageLayout[0];

			_pageLayout.removeChildAt(0);

			delete oldPage;
		}

		// Adding new page
		if (_route)
			_pageLayout += _route->buildElement();
	}

	OpenMenuButton& RC::getOpenMenuButton() {
		return _openMenuButton;
	}

	SX1262Transceiver& RC::getTransceiver() {
		return _transceiver;
	}
	
	RemotePacketHandler& RC::getPacketHandler() {
		return _packetHandler;
	}
	
	void RC::startErrorLoop(const char* error) {
		ESP_LOGE(_logTag, "%s", error);
		
		while (true) {
			vTaskDelay(pdMS_TO_TICKS(1'000));
		}
	}
}