#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../../../vector3.h"
#include "../../../../../sinAndCos.h"
#include "../../../../../geographicCoordinates.h"

#include "../../../../spatial/object.h"
#include "../../../../spatial/spatialView.h"
#include "../../../../spatial/camera.h"

#include "../../../../theme.h"

#include "objects/aircraft.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ND : public SpatialView {
		public:
			ND();

			constexpr static const uint32_t cameraAltitudeMinimum = 50;
			constexpr static const uint32_t cameraAltitudeMaximum = GeographicCoordinates::equatorialRadiusMeters * 2;

			const GeographicCoordinates& getCameraCoordinates() const;
			void setCameraCoordinates(const GeographicCoordinates& value);
			void resetCameraLatLon();

		protected:
			void onTick() override;

			void onEvent(Event* event) override;

		private:
			float _pinchLength = 0;
			Point _touchDownPosition;

			GeographicCoordinates _cameraCoordinates {
				0,
				0,
				500
			};

			Aircraft _aircraftObject {};

			float getRadiansPerPixelX();
			float getMetersPerPixelX();
			void setAltitudeFromDeltaPixels(float deltaPixels);
	};
}