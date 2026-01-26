#pragma once

#include <cstdint>
#include <array>

#include "types/generic.h"

namespace pizda {
	class RemoteDataLights {
		public:
			bool navigation = false;
			bool strobe = false;
			bool landing = false;
			bool cabin = false;
	};
	
	class RemoteDataAutopilot {
		public:
			float altitudeHoldFt = 0;
			
			AutopilotLateralMode lateralMode = AutopilotLateralMode::man;
			AutopilotVerticalMode verticalMode = AutopilotVerticalMode::man;
			
			bool autothrottle = false;
			bool autopilot = false;
	};

	class RemoteDataRadioSpectrumScanningFrequency {
		public:
			uint32_t from = 0;
			uint32_t to = 0;
			uint16_t step = 0;
			uint32_t value = 0;
	};

	enum class RemoteDataRadioSpectrumScanningState {
		none,
		requested,
		inProgress
	};

	class RemoteDataRadioSpectrumScanning {
		public:
			RemoteDataRadioSpectrumScanningState state = RemoteDataRadioSpectrumScanningState::none;
			RemoteDataRadioSpectrumScanningFrequency frequency {};
			std::array<int8_t, 512> history {};
	};

	class RemoteDataRadio {
		public:
			RemoteDataRadioSpectrumScanning spectrumScanning {};
	};
	
	class RemoteData {
		public:
			uint8_t throttle_0_255 = 0;
			
			RemoteDataLights lights {};
			RemoteDataAutopilot autopilot {};
			AircraftCalibrationSystem calibrationSystem = AircraftCalibrationSystem::accelAndGyro;
			RemoteDataRadio radio {};
	};
}