#pragma once

#include "resources/images/splashScreenImage.h"

#include "resources/images/menuIconMFDImage.h"
#include "resources/images/menuIconMFDPFDImage.h"
#include "resources/images/menuIconMFDNDImage.h"
#include "resources/images/menuIconMFDMainImage.h"
#include "resources/images/menuIconMFDAutopilotImage.h"
#include "resources/images/menuIconMFDAutopilotFlightDirectorImage.h"
#include "resources/images/menuIconMFDAutopilotEngageImage.h"
#include "resources/images/menuIconMFDBaroImage.h"
#include "resources/images/menuIconMFDTrimImage.h"

#include "resources/images/menuIconMFDLightsImage.h"
#include "resources/images/menuIconMFDLightsNavigationImage.h"
#include "resources/images/menuIconMFDLightsStrobeImage.h"
#include "resources/images/menuIconMFDLightsLandingImage.h"
#include "resources/images/menuIconMFDLightsCabinImage.h"

#include "resources/images/menuIconFlightPlanImage.h"
#include "resources/images/menuIconWaypointsImage.h"

#include "resources/images/menuIconPersonalizationImage.h"
#include "resources/images/menuIconCalibrationImage.h"
#include "resources/images/menuIconMotorsImage.h"
#include "resources/images/menuIconAxesImage.h"
#include "resources/images/menuIconDevImage.h"

namespace pizda::resources {
	class images {
		public:
			constexpr static SplashScreenImage splashScreen {};

			constexpr static MenuIconMFDImage menuIconMFD {};
			constexpr static MenuIconMFDPFDImage menuIconMFDPFD {};
			constexpr static MenuIconMFDNDImage menuIconMFDND {};
			constexpr static MenuIconMFDAutopilotImage menuIconMFDAutopilot {};
			constexpr static MenuIconMFDAutopilotFlightDirectorImage menuIconMFDAutopilotFlightDirector {};
			constexpr static MenuIconMFDAutopilotEngageImage menuIconMFDAutopilotEngage {};
			constexpr static MenuIconMFDBaroImage menuIconMFDBaro {};
			constexpr static MenuIconMFDTrimImage menuIconMFDTrim {};
			constexpr static MenuIconMFDLightsImage menuIconMFDLights {};
			constexpr static MenuIconMFDLightsNavigationImage menuIconMFDLightsNavigation {};
			constexpr static MenuIconMFDLightsStrobeImage menuIconMFDLightsStrobe {};
			constexpr static MenuIconMFDLightsLandingImage menuIconMFDLightsLanding {};
			constexpr static MenuIconMFDLightsCabinImage menuIconMFDLightsCabin {};
			
			constexpr static MenuIconFlightPlanImage menuIconFlightPlan {};
			constexpr static MenuIconWaypointsImage menuIconWaypointsImage {};

			constexpr static MenuIconPersonalizationImage menuIconPersonalization {};
			constexpr static MenuIconCalibrationImage menuIconCalibration {};
			constexpr static MenuIconMotorsImage menuIconMotors {};
			constexpr static MenuIconAxesImage menuIconAxes {};
			constexpr static MenuIconDevImage menuIconDev {};
	};
}