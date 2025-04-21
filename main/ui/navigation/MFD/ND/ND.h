#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../../vector3.h"
#include "../../../../sinAndCos.h"
#include "../../../../geographicCoordinates.h"

#include "../../../spatial/element.h"
#include "../../../spatial/spatialView.h"
#include "../../../spatial/camera.h"

#include "../../../theme.h"

#include "elements/aircraft.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

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