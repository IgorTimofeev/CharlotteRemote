#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"
#include "../../theme.h"
#include "../../../vector3.h"
#include "../../../sinAndCos.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ND : public Element {
		public:
			ND();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		protected:
			void onEvent(Event& event) override;

		private:
			constexpr static const float _earthEquatorialRadius = 6378137;
			constexpr static const float _earthEquatorialLength = 2 * M_PI * _earthEquatorialRadius;
			constexpr static const float _earthMetersPer1EquatorialDeg = _earthEquatorialLength / 360;

			// Testing on Google Maps with scale 60px = 200m
			// Kronshtadt runway length is ~500m long
			// So to fit it on 240px remote screen, MPP should be like
			float _metersPerPixel = 240.f * 200.f / 60.f;

			float _pinchDownPixelsPerMeter = 0;
			int32_t _pinchDownLength = 0;
			Point _touchDownPosition;
			Vector2F _cameraOffset;

			std::vector<Vector3F> _points {
				// Airfield west corner
				Vector3F(
					60.01483325540486f,
					29.69835915766679f,
					0.0f
				),
				// Airfield east corner
				Vector3F(
					60.014390616612474f,
					29.706975357970624f,
					0.0f
				),
			};

			static Vector3F geographicToCartesian(const SinAndCos& latitude, const SinAndCos& longitude, float distanceToEarthCenter);
	};
}