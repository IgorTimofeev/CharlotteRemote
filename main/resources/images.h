#pragma once

#include "images/splashScreenImage.h"

#include "images/menuIconMFDImage.h"
#include "images/menuIconMFDPFDImage.h"
#include "images/menuIconMFDNDImage.h"
#include "images/menuIconMFDMainImage.h"
#include "images/menuIconMFDAutopilotImage.h"
#include "images/menuIconMFDPressureImage.h"

#include "images/menuIconFlightPlanImage.h"
#include "images/menuIconWaypointsImage.h"

#include "images/menuIconPersonalizationImage.h"
#include "images/menuIconAxisImage.h"
#include "images/menuIconDevImage.h"
#include "images/menuIconPowerImage.h"

#include "images/MFDFlapsAndSpoilersImage.h"
#include "images/MFDLandingGearExtendedImage.h"
#include "images/MFDLandingGearRetractedImage.h"

namespace pizda::resources {
	class Images {
		public:
			static const SplashScreenImage splashScreen;

			static const MenuIconMFDImage menuIconMFD;
			static const MenuIconMFDPFDImage menuIconMFDPFD;
			static const MenuIconMFDNDImage menuIconMFDND;
			static const MenuIconMFDMainImage menuIconMFDMain;
			static const MenuIconMFDAutopilotImage menuIconMFDAutopilot;
			static const MenuIconMFDPressureImage menuIconMFDPressure;

			static const MenuIconFlightPlanImage menuIconFlightPlan;
			static const MenuIconWaypointsImage menuIconWaypointsImage;

			static const MenuIconPersonalizationImage menuIconPersonalization;
			static const MenuIconAxisImage menuIconAxis;
			static const MenuIconDevImage menuIconDev;
			static const MenuIconPowerImage menuIconPower;

			static const MFDFlapsAndSpoilersImage MFDFlapsAndSpoilers;
			static const MFDLandingGearExtendedImage MFDLandingGearExtended;
			static const MFDLandingGearRetractedImage MFDLandingGearRetracted;
	};
}