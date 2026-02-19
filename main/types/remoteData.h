#pragma once

#include <cstdint>
#include <array>
#include <limits>

#include "types/generic.h"

namespace pizda {
	class RemoteDataLights {
		public:
			bool navigation = false;
			bool strobe = false;
			bool landing = false;
			bool cabin = false;
	};


	class RemoteDataAutopilotPID {
		public:
			AutopilotPIDType type = AutopilotPIDType::rollToAilerons;
			PIDCoefficients coefficients {};
	};
	
	class RemoteDataAutopilot {
		public:
			float altitudeHoldFt = 0;
			
			AutopilotLateralMode lateralMode = AutopilotLateralMode::dir;
			AutopilotVerticalMode verticalMode = AutopilotVerticalMode::dir;

			bool autothrottle = false;
			bool autopilot = false;

			RemoteDataAutopilotPID PID {};
	};

	enum class RemoteDataRadioSpectrumScanningState {
		stopped,
		startRequested,
		started,
		stopRequested
	};

	class RemoteDataTransceiverSpectrumScanning {
		public:
			RemoteDataTransceiverSpectrumScanning() {
				// Clearing history
				for (auto& record : history)
					record = std::numeric_limits<int8_t>::min();
			}

			RemoteDataRadioSpectrumScanningState state = RemoteDataRadioSpectrumScanningState::stopped;
			uint32_t frequency = 0;
			std::array<int8_t, 255> history {};
			int8_t historyMin = 0;
			int8_t historyMax = std::numeric_limits<int8_t>::min();
	};

	class RemoteDataTransceiver {
		public:
			TransceiverCommunicationSettings communicationSettings {};
			RemoteDataTransceiverSpectrumScanning spectrumScanning {};
	};
	
	class RemoteData {
		public:
			RemoteDataLights lights {};
			RemoteDataAutopilot autopilot {};
			AircraftCalibrationSystem calibrationSystem = AircraftCalibrationSystem::accelAndGyro;
			RemoteDataTransceiver transceiver {};
	};
}