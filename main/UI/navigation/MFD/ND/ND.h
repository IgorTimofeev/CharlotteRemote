#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../../theme.h"
#include "aircraft.h"

namespace pizda {
	using namespace YOBA;

	class ND : public SpatialView {
		public:
			ND();

			~ND();

			constexpr static const uint32_t cameraAltitudeMinimum = 50;
			constexpr static const uint32_t cameraAltitudeMaximum = GeographicCoordinates::equatorialRadiusMeters * 2;

			const GeographicCoordinates& getCameraOffset() const;
			void setCameraOffset(const GeographicCoordinates& value);
			void resetCameraOffsetLatLon();

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

			GeographicCoordinates _cameraCoordinates {};

			AircraftElement* _aircraftElement = nullptr;

			float getEquatorialRadiansPerPixel();
			void computeCameraCoordinates();
	};
}