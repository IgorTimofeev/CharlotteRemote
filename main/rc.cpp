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
		multicoreSetup();
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
		ADCSetup();

		// Settings contain calibration data for the ADC axes, so they should be read first
		_settings.readAll();

		// HID
		_touchPanel.setup();

		_encoder.setup();
		_encoder.setMinimumDelta(4);

		_axes.setup();
		_battery.setup();
		_audioPlayer.setup();

		// Transceiver
		if (!_transceiver.setup())
			startErrorLoop("failed to setup XCVR");
		
		// Application
		_application.setRenderer(&_renderer);
		_application.addHID(&_touchPanel);
		_application.addHID(&_encoder);

		// UI
		UISetup();

		// Beep beep
		_audioPlayer.play(resources::sounds::boot);

		// This shit is blazingly 🔥 fast 🚀, so letting user enjoy logo for a few moments
		vTaskDelay(pdMS_TO_TICKS(500));

		// Main loop, we'll use it for UI
		while (true) {
			_axes.tick();
			_battery.tick();=

			interpolateData();

			_application.tick();
			_application.render();

			vTaskDelay(pdMS_TO_TICKS(1'000 / config::application::interfaceTickRateHz));
		}
	}

	float RC::applyLPF(const float oldValue, const float newValue, const float factor) const {
		return
			_settings.personalization.LPF
			? LowPassFilter::apply(oldValue, newValue, factor)
			: newValue;
	}
	
	float RC::applyLPFToAngle(const float oldValue, const float newValue, const float factor) const {
		return
			_settings.personalization.LPF
			? LowPassFilter::applyToAngle(oldValue, newValue, factor)
			: newValue;
	}
	
	void RC::interpolateData() {
		const auto deltaTimeUs = esp_timer_get_time() - _dataInterpolationTime;
		_dataInterpolationTime = esp_timer_get_time();

		// Principle of calculating the interpolation factor:
		//
		// factorPerSecond -> 1'000'000 us
		// factorPerTick -> deltaTime us
		//
		// factorPerTick = factorPerSecond * deltaTime / 1'000'000

		// Fast
		float LPFFactor = LowPassFilter::getDeltaTimeFactor(5.0f, deltaTimeUs);
		
		// Pitch
		_aircraftData.computed.pitchRad = applyLPFToAngle(
			_aircraftData.computed.pitchRad,
			_aircraftData.raw.pitchRad,
			LPFFactor
		);
		
		// Roll
		_aircraftData.computed.rollRad = applyLPFToAngle(
			_aircraftData.computed.rollRad,
			_aircraftData.raw.rollRad,
			LPFFactor
		);
		
		// Yaw
		_aircraftData.computed.yawRad = applyLPFToAngle(
			_aircraftData.computed.yawRad,
			_aircraftData.raw.yawRad,
			LPFFactor
		);
		
		// Heading
		_aircraftData.computed.headingDeg = normalizeAngleDeg360(toDegrees(-_aircraftData.computed.yawRad));
//		ESP_LOGI("PIZDA", "raw: %f, raw deg: %f, computed: %f, deg: %f, heading: %f", _aircraftData.raw.yawRad, toDegrees(-_aircraftData.raw.yawRad), _aircraftData.computed.yawRad, toDegrees(_aircraftData.computed.yawRad), _aircraftData.computed.headingDeg);

		// Coordinates
		_aircraftData.computed.coordinates.setLatitude(applyLPF(
			_aircraftData.computed.coordinates.getLatitude(),
			_aircraftData.raw.coordinates.getLatitude(),
			LPFFactor
		));

		_aircraftData.computed.coordinates.setLongitude(applyLPF(
			_aircraftData.computed.coordinates.getLongitude(),
			_aircraftData.raw.coordinates.getLongitude(),
			LPFFactor
		));

		_aircraftData.computed.coordinates.setAltitude(applyLPF(
			_aircraftData.computed.coordinates.getAltitude(),
			_aircraftData.raw.coordinates.getAltitude(),
			LPFFactor
		));

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
		LPFFactor = LowPassFilter::getDeltaTimeFactor(3.0f, deltaTimeUs);
		
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
		LPFFactor = LowPassFilter::getDeltaTimeFactor(1.0f, deltaTimeUs);
		
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
		LPFFactor = LowPassFilter::getDeltaTimeFactor(0.5f, deltaTimeUs);
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
	
	SemaphoreHandle_t RC::getSPIMutex() const {
		return _SPIMutex;
	}

	Settings& RC::getSettings() {
		return _settings;
	}
	
	AudioPlayer& RC::getAudioPlayer() {
		return _audioPlayer;
	}

	PushButtonEncoder& RC::getEncoder() {
		return _encoder;
	}

	Axes& RC::getAxes() {
		return _axes;
	}

	Battery<
		config::battery::remote::unit,
		config::battery::remote::channel,
		config::battery::remote::voltageMin,
		config::battery::remote::voltageMax,
		config::battery::remote::voltageDividerR1,
		config::battery::remote::voltageDividerR2
	>
	RC::getBattery() const {
		return _battery;
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

	void RC::multicoreSetup() {
		_SPIMutex = xSemaphoreCreateMutex();
		system::SPI::setMutex(_SPIMutex);
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

	void RC::ADCSetup() {
		adc_oneshot_unit_init_cfg_t unitConfig {};
		unitConfig.unit_id = ADC_UNIT_1;
		unitConfig.clk_src = ADC_RTC_CLK_SRC_DEFAULT;
		unitConfig.ulp_mode = ADC_ULP_MODE_DISABLE;
		ESP_ERROR_CHECK(adc_oneshot_new_unit(&unitConfig, &_ADCOneshotUnit1));
	}

	void RC::UISetup() {
		_application.setBackgroundColor(&Theme::bg1);
		_application += &_pageLayout;
		_application += &_openMenuButton;

		setRoute(&Routes::MFD);
		updateDebugOverlayVisibility();
	}

	void RC::GPIOSetup() {
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
	
	RemoteTransceiver& RC::getTransceiver() {
		return _transceiver;
	}

	[[noreturn]] void RC::startErrorLoop(const char* error) {
		ESP_LOGE(_logTag, "%s", error);
		
		while (true) {
			vTaskDelay(pdMS_TO_TICKS(1'000));
		}
	}
}