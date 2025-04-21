#pragma once

#include "images/menuIconMFDImage.h"
#include "images/menuIconMFDNDImage.h"
#include "images/menuIconMFDMainImage.h"
#include "images/menuIconMFDAutopilotImage.h"
#include "images/menuIconMFDPressureImage.h"
#include "images/menuIconSettingsImage.h"
#include "images/menuIconWiFiImage.h"
#include "images/menuIconAxisImage.h"
#include "images/menuIconDevImage.h"

#include "images/MFDFlapsAndSpoilersImage.h"
#include "images/MFDLandingGearExtendedImage.h"
#include "images/MFDLandingGearRetractedImage.h"

namespace pizda::resources {
	class Images {
		public:
			static const MenuIconMFDImage menuIconMFD;
			static const MenuIconMFDNDImage menuIconMFDND;
			static const MenuIconMFDMainImage menuIconMFDMain;
			static const MenuIconMFDAutopilotImage menuIconMFDAutopilot;
			static const MenuIconMFDPressureImage menuIconMFDPressure;
			static const MenuIconSettingsImage menuIconSettings;
			static const MenuIconWiFiImage menuIconWiFi;
			static const MenuIconAxisImage menuIconAxis;
			static const MenuIconDevImage menuIconDev;

			static const MFDFlapsAndSpoilersImage MFDFlapsAndSpoilers;
			static const MFDLandingGearExtendedImage MFDLandingGearExtended;
			static const MFDLandingGearRetractedImage MFDLandingGearRetracted;
	};
}