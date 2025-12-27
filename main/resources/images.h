#pragma once

#include "images/splashScreenImage.h"

#include "images/menuIconMFDImage.h"
#include "images/menuIconMFDPFDImage.h"
#include "images/menuIconMFDNDImage.h"
#include "images/menuIconMFDMainImage.h"
#include "images/menuIconMFDAutopilotImage.h"
#include "images/menuIconMFDBaroImage.h"
#include "images/menuIconMFDTrimImage.h"

#include "images/menuIconMFDLightsImage.h"
#include "images/menuIconMFDLightsNavigationImage.h"
#include "images/menuIconMFDLightsStrobeImage.h"
#include "images/menuIconMFDLightsLandingImage.h"

#include "images/menuIconFlightPlanImage.h"
#include "images/menuIconWaypointsImage.h"

#include "images/menuIconPersonalizationImage.h"
#include "images/menuIconAxisImage.h"
#include "images/menuIconDevImage.h"
#include "images/menuIconPowerImage.h"

namespace pizda::resources {
	class Images {
		public:
			static const SplashScreenImage splashScreen;

			static const MenuIconMFDImage menuIconMFD;
			static const MenuIconMFDPFDImage menuIconMFDPFD;
			static const MenuIconMFDNDImage menuIconMFDND;
			static const MenuIconMFDAutopilotImage menuIconMFDAutopilot;
			static const MenuIconMFDBaroImage menuIconMFDBaro;
			static const MenuIconMFDTrimImage menuIconMFDTrim;
			static const MenuIconMFDLightsImage menuIconMFDLights;
			static const MenuIconMFDLightsNavigationImage menuIconMFDLightsNavigation;
			static const MenuIconMFDLightsStrobeImage menuIconMFDLightsStrobe;
			static const MenuIconMFDLightsLandingImage menuIconMFDLightsLanding;
			
			static const MenuIconFlightPlanImage menuIconFlightPlan;
			static const MenuIconWaypointsImage menuIconWaypointsImage;

			static const MenuIconPersonalizationImage menuIconPersonalization;
			static const MenuIconAxisImage menuIconAxis;
			static const MenuIconDevImage menuIconDev;
			static const MenuIconPowerImage menuIconPower;
	};
}