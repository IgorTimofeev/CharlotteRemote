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
		_renderer.renderImage(Point(), &resources::Images::splashScreen);
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
		_speaker.setup();

		_leverLeft.setup();
		_leverRight.setup();
		_joystickHorizontal.setup();
		_joystickVertical.setup();
		_ring.setup();

		// Transceiver
		if (!_transceiver.setup())
			startErrorLoop("failed to setup XCVR");
		
		_transceiver.setPacketHandler(&_packetHandler);
		_transceiver.start();
		
		// Application
		_application.setRenderer(&_renderer);
		_application.addHID(&_touchPanel);
		_application.addHID(&_encoder);

		// UI
		_application.setBackgroundColor(&Theme::bg1);
		_application += &_pageLayout;
		_openMenuButton.applyBottomStyle();
		_application += &_openMenuButton;

		setRoute(&Routes::MFD);
		updateDebugOverlayVisibility();

		// This shit is blazingly 🔥 fast 🚀, so letting user enjoy logo for a few moments
		vTaskDelay(pdMS_TO_TICKS(1000));

		// Beep beep
		_speaker.play(resources::Sounds::boot());

		// Main loop
		while (true) {
			const auto time = esp_timer_get_time();

			// High priority tasks
			axisTick();
			interpolationTick();

			// UI
			_application.tick();
			_application.render();

			// Low priority tasks
			_speaker.tick();

// 			_tickDeltaTime = esp_timer_get_time() - time;
//
// 			// Skipping remaining tick time if any
// 			if (_tickDeltaTime < constants::application::mainTickInterval) {
// 				// FreeRTOS tasks can be only delayed by ms, so...
// 				vTaskDelay(pdMS_TO_TICKS((constants::application::mainTickInterval - _tickDeltaTime) / 1000));
//
// //				ESP_LOGI("Main", "Skipping ticks for %lu ms", (constants::application::mainTickInterval - _tickDeltaTime) / 1000);
// 			}
		}
	}

	void RC::interpolationTick() {
		const uint32_t deltaTime = esp_timer_get_time() - _interpolationTickTime;

		if (deltaTime < config::application::interpolationTickInterval)
			return;

		// Principle of calculating the interpolation factor:
		//
		// factorPerSecond -> 1'000'000 us
		// factorPerTick -> deltaTime us
		//
		// factorPerTick = factorPerSecond * deltaTime / 1'000'000

		// Roll / pitch / yaw / slip & skid, faster
		float LPFFactor = 5.0f * static_cast<float>(deltaTime) / 1'000'000.f;
		LowPassFilter::apply(_aircraftData.computed.pitch, _aircraftData.pitchRad, LPFFactor);
		LowPassFilter::apply(_aircraftData.computed.roll, _aircraftData.rollRad, LPFFactor);
		LowPassFilter::apply(_aircraftData.computed.yaw, _aircraftData.yawRad, LPFFactor);

		LowPassFilter::apply(_aircraftData.computed.slipAndSkid, _aircraftData.slipAndSkid, LPFFactor);

		LowPassFilter::apply(_aircraftData.computed.flightPathVectorPitch, _aircraftData.flightPathVectorPitch, LPFFactor);
		LowPassFilter::apply(_aircraftData.computed.flightPathVectorYaw, _aircraftData.flightPathVectorYaw, LPFFactor);

		LowPassFilter::apply(_aircraftData.computed.flightDirectorPitch, _aircraftData.flightDirectorPitch, LPFFactor);
		LowPassFilter::apply(_aircraftData.computed.flightDirectorRoll, _aircraftData.flightDirectorRoll, LPFFactor);

		// Airspeed / altitude, normal
		LPFFactor = 3.0f * static_cast<float>(deltaTime) / 1'000'000.f;
		LowPassFilter::apply(_aircraftData.computed.airSpeed, _aircraftData.airSpeedKt, LPFFactor);
		LowPassFilter::apply(_aircraftData.computed.altitude, _aircraftData.altitudeFt, LPFFactor);
		LowPassFilter::apply(_aircraftData.computed.windDirection, _aircraftData.windDirection, LPFFactor);

		// Trends, slower
		LPFFactor = 1.0f * static_cast<float>(deltaTime) / 1'000'000.f;
		LowPassFilter::apply(_aircraftData.computed.airSpeedTrend, _aircraftData.airSpeedTrend, LPFFactor);
		LowPassFilter::apply(_aircraftData.computed.altitudeTrend, _aircraftData.altitudeTrend, LPFFactor);

		// Smooth as fuck
		LPFFactor = 0.5f * static_cast<float>(deltaTime) / 1'000'000.f;
		LowPassFilter::apply(_aircraftData.computed.transceiverRSSI, _transceiver.getRSSI(), LPFFactor);

		_interpolationTickTime = esp_timer_get_time() + config::application::interpolationTickInterval;
	}

	// ------------------------- Data -------------------------

	Application& RC::getApplication() {
		return _application;
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

	Speaker& RC::getSpeaker() {
		return _speaker;
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
		if (esp_timer_get_time() < _axisTickTime)
			return;

		_leverLeft.tick();
		_leverRight.tick();
		_joystickHorizontal.tick();
		_joystickVertical.tick();
		_ring.tick();
		_battery.tick();

		_axisTickTime = esp_timer_get_time() + config::axis::tickInterval;
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
		if (_settings.interface.developer.debugOverlay) {
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

	Transceiver& RC::getTransceiver() {
		return _transceiver;
	}
	
	void RC::startErrorLoop(const char* error) {
		ESP_LOGE(_logTag, "%s", error);
		
		while (true) {
			vTaskDelay(pdMS_TO_TICKS(1'000));
		}
	}
}