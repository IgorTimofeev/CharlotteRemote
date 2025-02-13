#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "../../theme.h"
#include "../../spatial/vector3.h"
#include "../../../sinAndCos.h"

#include "../../spatial/spatialView.h"
#include "../../spatial/camera.h"
#include "../../spatial/object.h."
#include "../../spatial/vector3.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ND : public SpatialView {
		public:
			ND();

		protected:
			void onTick() override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event* event) override;

		private:
//			constexpr static const float _earthEquatorialRadius = 6378137;
			constexpr static const float _earthEquatorialRadius = 100;
			constexpr static const float _earthEquatorialLength = M_PI * 2 * _earthEquatorialRadius;
			constexpr static const float _earthMetersPer1EquatorialDeg = _earthEquatorialLength / 360;

			// Testing on Google Maps with scale 60px = 200m
			// Kronshtadt runway length is ~500m long
			// So to fit it on 240px remote screen, MPP should be like
			float _metersPerPixel = 240.f * 200.f / 60.f;

			float _pinchDownPixelsPerMeter = 0;
			int32_t _pinchDownLength = 0;
			Point _touchDownPosition;
			Vector3F _cameraOffset {};

			static Vector3F geographicToCartesian(const SinAndCos& latitude, const SinAndCos& longitude, float distanceToEarthCenter);
	};
}