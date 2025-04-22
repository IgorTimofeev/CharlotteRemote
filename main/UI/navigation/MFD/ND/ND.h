#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../../../geographicCoordinates.h"

#include "../../../spatial/element.h"
#include "../../../spatial/spatialView.h"
#include "../../../spatial/camera.h"

#include "../../../theme.h"

#include "elements/aircraft.h"

namespace pizda {
	using namespace YOBA;

	class ND : public SpatialView {
		public:
			ND();

			constexpr static const uint32_t cameraAltitudeMinimum = 50;
			constexpr static const uint32_t cameraAltitudeMaximum = GeographicCoordinates::equatorialRadiusMeters * 2;

			const GeographicCoordinates& getCameraOffset() const;
			void setCameraOffset(const GeographicCoordinates& value);
			void resetCameraOffsetLatLon();
			GeographicCoordinates getCameraCoordinates();

		protected:
			void onTick() override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

		private:
			float _pinchLength = 0;
			Point _touchDownPosition {};
			Point _cursorPosition {};

			GeographicCoordinates _cameraOffset {
				0,
				0,
				500
			};

			AircraftElement _aircraftElement {};

			float getEquatorialRadiansPerPixel();
	};
}