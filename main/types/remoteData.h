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

	enum class RemoteDataRadioSpectrumScanningState {
		none,
		requested,
		inProgress
	};

	class RemoteDataRadioSpectrumScanningHistoryRecord {
		public:
			int8_t RSSI;
			uint8_t saturation;
	};

	class RemoteDataRadioSpectrumScanning {
		public:
			RemoteDataRadioSpectrumScanningState state = RemoteDataRadioSpectrumScanningState::none;
			uint32_t frequency = 0;
			std::array<RemoteDataRadioSpectrumScanningHistoryRecord, 512> history {};
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
			RemoteDataRadio transceiver {};
	};
}