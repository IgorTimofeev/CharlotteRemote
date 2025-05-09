#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/UI/spatial.h>

#include "../../../theme.h"
#include "aircraft.h"

namespace pizda {
	using namespace YOBA;
	using namespace YOBA::spatial;

	class ND : public Scene {
		public:
			ND();
			~ND() override;

			constexpr static uint32_t cameraAltitudeMinimum = 50;
			constexpr static uint32_t cameraAltitudeMaximum = GeographicCoordinates::equatorialRadiusMeters * 2;

			const GeographicCoordinates& getCameraOffset() const;
			void setCameraOffset(const GeographicCoordinates& value);
			void resetCameraOffsetLatLon();

		protected:
			void onTick() override;
			void onBoundsChanged() override;
			void onRender(Renderer* renderer) override;
			void onEvent(Event* event) override;

		private:
			constexpr static uint8_t _compassAngleStepUnitsDeg = 10;
			constexpr static uint8_t _compassAngleStepUnitsBigDeg = 30;

			constexpr static uint8_t _compassHeadingTextMarginTop = 3;
			constexpr static uint8_t _compassHeadingTextHorizontalLineOffset = 1;
			constexpr static uint8_t _compassHeadingTextVerticalLineHeight = 2;

			constexpr static uint8_t _compassMarginTop = 4;

			constexpr static uint8_t _compassArcLandscapeMarginHorizontal = 15;
			constexpr static uint8_t _compassArcMarginBottom = 20;
			constexpr static uint16_t _compassArcViewportDeg = _compassAngleStepUnitsDeg * 10;
			constexpr static uint16_t _compassArcViewportHalfDeg = _compassArcViewportDeg / 2;

			constexpr static uint8_t _compassAngleStepLineSmallLength = 3;
			constexpr static uint8_t _compassAngleStepLineBigLength = 5;
			constexpr static uint8_t _compassAngleStepLineTextOffset = 3;

			static GeographicCoordinates _cameraOffset;
			GeographicCoordinates _cameraCoordinates {};

			float _pinchLength = 0;
			Point _touchDownPosition {};
			Point _cursorPosition { -1, -1 };

			AircraftElement* _aircraftElement = nullptr;

			float getEquatorialRadiansPerPixel() const;
	};
}