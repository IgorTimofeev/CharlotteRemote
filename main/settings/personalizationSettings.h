#pragma once

#include <cstdint>

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;
	
	class PersonalizationSettingsMFDPFD {
		public:
			bool visible = false;
			uint8_t FOV = 0;
			bool flightDirector = false;
			bool waypointLabels = true;
	};

	enum class PersonalizationSettingsMFDNDMode : uint8_t {
		arcHeadingUp,
		mapHeadingUp,

		last = mapHeadingUp
	};

	class PersonalizationSettingsMFDND {
		public:
			bool visible = true;
			PersonalizationSettingsMFDNDMode mode = PersonalizationSettingsMFDNDMode::arcHeadingUp;
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

	class PersonalizationSettingsMFD {
		public:
			PersonalizationSettingsMFDPFD PFD {};
			PersonalizationSettingsMFDND ND {};
			PersonalizationSettingsMFDToolbar toolbar {};
			uint8_t splitPercent = 60;

			bool isAnyPanelVisible() const {
				return PFD.visible || ND.visible;
			}
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
				MFD.PFD.visible = stream.readBool(_MFDPFDVisible, true);
				MFD.PFD.FOV = stream.readUint8(_MFDPFDFOV, 50);
				MFD.PFD.flightDirector = stream.readBool(_MFDPFDFlightDirectors, true);
				MFD.PFD.waypointLabels = stream.readBool(_MFDPFDWaypointLabels, true);

				MFD.ND.visible = stream.readBool(_MFDNDVisible, true);
				MFD.ND.mode = static_cast<PersonalizationSettingsMFDNDMode>(stream.readUint8(_MFDNDMode, static_cast<uint8_t>(PersonalizationSettingsMFDNDMode::arcHeadingUp)));

				MFD.ND.earth = stream.readBool(_MFDNDEarth, true);

				MFD.toolbar.mode = static_cast<PersonalizationSettingsMFDToolbarMode>(stream.readUint8(_MFDToolbarMode, static_cast<uint8_t>(PersonalizationSettingsMFDToolbarMode::none)));
				
				MFD.splitPercent = stream.readUint8(_MFDSplitPercent, 60);
				
				// Other
				LPF = stream.readBool(_LPF, true);
				audioFeedback = stream.readBool(_audioFeedback, true);
				debugOverlay = stream.readBool(_debugOverlay, false);
			}

			void onWrite(const NVSStream& stream) override {
				// MFD
				stream.writeBool(_MFDPFDVisible, MFD.PFD.visible);
				stream.writeUint8(_MFDPFDFOV, MFD.PFD.FOV);
				stream.writeBool(_MFDPFDFlightDirectors, MFD.PFD.flightDirector);
				stream.writeBool(_MFDPFDWaypointLabels, MFD.PFD.waypointLabels);

				stream.writeBool(_MFDNDVisible, MFD.ND.visible);
				stream.writeUint8(_MFDNDMode, static_cast<uint8_t>(MFD.ND.mode));

				stream.writeBool(_MFDNDEarth, MFD.ND.earth);

				stream.writeUint8(_MFDToolbarMode, static_cast<uint8_t>(MFD.toolbar.mode));

				stream.writeUint8(_MFDSplitPercent, MFD.splitPercent);
				
				// Other
				stream.writeBool(_LPF, LPF);
				stream.writeBool(_audioFeedback, audioFeedback);
				stream.writeBool(_debugOverlay, debugOverlay);
			}

		private:
			constexpr static auto _namespace = "pe0";
			constexpr static auto _MFDPFDVisible = "mpvi";
			constexpr static auto _MFDPFDFOV = "mpfo";
			constexpr static auto _MFDPFDFlightDirectors = "mpfd";
			constexpr static auto _MFDPFDWaypointLabels = "mpwl";
			constexpr static auto _MFDNDVisible = "mnvi";
			constexpr static auto _MFDNDMode = "mnmd";
			constexpr static auto _MFDNDEarth = "mnea";
			constexpr static auto _MFDToolbarMode = "mtmd";
			constexpr static auto _MFDSplitPercent = "msp";
			
			constexpr static auto _LPF = "lp";
			constexpr static auto _audioFeedback = "af";
			constexpr static auto _debugOverlay = "do";
	};
}
