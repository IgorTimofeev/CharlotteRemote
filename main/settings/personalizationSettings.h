#pragma once

#include <cstdint>

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
	class PersonalizationSettingsMFDPFD {
		public:
			uint8_t FOV = 0;
			bool flightDirector = false;
			bool metricUnits = false;
			bool waypointLabels = true;
	};

	enum class PersonalizationSettingsMFDNDMode : uint8_t {
		arc,
		map,

		maxValue = map
	};

	class PersonalizationSettingsMFDND {
		public:
			PersonalizationSettingsMFDNDMode mode = PersonalizationSettingsMFDNDMode::arc;
			bool earth = true;
	};

	enum class PersonalizationSettingsMFDToolbarMode : uint8_t {
		none,
		autopilot,
		baro,
		trim,
		lights
	};

	class PersonalizationSettingsMFDToolbar {
		public:
			PersonalizationSettingsMFDToolbarMode mode = PersonalizationSettingsMFDToolbarMode::none;
	};

	enum class PersonalizationSettingsMFDSplitMode : uint8_t {
		PFD,
		ND,
		split
	};

	class PersonalizationSettingsMFDSplit {
		public:
			PersonalizationSettingsMFDSplitMode mode = PersonalizationSettingsMFDSplitMode::PFD;
			// Percent
			uint8_t ratio = 60;
	};

	class PersonalizationSettingsMFD {
		public:
			PersonalizationSettingsMFDPFD PFD {};
			PersonalizationSettingsMFDND ND {};
			PersonalizationSettingsMFDToolbar toolbar {};
			PersonalizationSettingsMFDSplit split {};
	};
	
	class PersonalizationSettings : public NVSSettings {
		public:
			PersonalizationSettingsMFD MFD {};
			bool LPF = false;
			bool debugOverlay = false;
			bool audioFeedback = false;
		
		protected:
			const char* getNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				// MFD
				MFD.PFD.FOV = stream.readUint8(_MFDPFDFOV, 50);
				MFD.PFD.flightDirector = stream.readBool(_MFDPFDFlightDirector, true);
				MFD.PFD.metricUnits = stream.readBool(_MFDPFDFMetricUnits, false);
				MFD.PFD.waypointLabels = stream.readBool(_MFDPFDWaypointLabels, true);

				MFD.ND.mode = static_cast<PersonalizationSettingsMFDNDMode>(stream.readUint8(_MFDNDMode, static_cast<uint8_t>(PersonalizationSettingsMFDNDMode::arc)));
				MFD.ND.earth = stream.readBool(_MFDNDEarth, true);

				MFD.toolbar.mode = static_cast<PersonalizationSettingsMFDToolbarMode>(stream.readUint8(_MFDToolbarMode, static_cast<uint8_t>(PersonalizationSettingsMFDToolbarMode::none)));

				MFD.split.mode = static_cast<PersonalizationSettingsMFDSplitMode>(stream.readUint8(_MFDSplitMode, static_cast<uint8_t>(PersonalizationSettingsMFDSplitMode::PFD)));
				MFD.split.ratio = stream.readUint8(_MFDSplitRatio, 60);

				// Other
				LPF = stream.readBool(_LPF, true);
				audioFeedback = stream.readBool(_audioFeedback, true);
				debugOverlay = stream.readBool(_debugOverlay, false);
			}

			void onWrite(const NVSStream& stream) override {
				// MFD
				stream.writeUint8(_MFDPFDFOV, MFD.PFD.FOV);
				stream.writeBool(_MFDPFDFlightDirector, MFD.PFD.flightDirector);
				stream.writeBool(_MFDPFDFMetricUnits, MFD.PFD.metricUnits);
				stream.writeBool(_MFDPFDWaypointLabels, MFD.PFD.waypointLabels);

				stream.writeUint8(_MFDNDMode, static_cast<uint8_t>(MFD.ND.mode));
				stream.writeBool(_MFDNDEarth, MFD.ND.earth);

				stream.writeUint8(_MFDToolbarMode, static_cast<uint8_t>(MFD.toolbar.mode));

				stream.writeUint8(_MFDSplitMode, static_cast<uint8_t>(MFD.split.mode));
				stream.writeUint8(_MFDSplitRatio, MFD.split.ratio);

				// Other
				stream.writeBool(_LPF, LPF);
				stream.writeBool(_audioFeedback, audioFeedback);
				stream.writeBool(_debugOverlay, debugOverlay);
			}

		private:
			constexpr static auto _namespace = "pe0";
			constexpr static auto _MFDPFDFOV = "mpfo";
			constexpr static auto _MFDPFDFlightDirector = "mpfd";
			constexpr static auto _MFDPFDFMetricUnits = "mpmu";
			constexpr static auto _MFDPFDWaypointLabels = "mpwl";
			constexpr static auto _MFDNDMode = "mnmd";
			constexpr static auto _MFDNDEarth = "mnea";
			constexpr static auto _MFDToolbarMode = "mtmd";
			constexpr static auto _MFDSplitMode = "msm";
			constexpr static auto _MFDSplitRatio = "msr";

			constexpr static auto _LPF = "lp";
			constexpr static auto _audioFeedback = "af";
			constexpr static auto _debugOverlay = "do";
	};
}
