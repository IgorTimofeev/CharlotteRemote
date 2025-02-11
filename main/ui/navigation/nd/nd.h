#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"
#include "../../theme.h"
#include "../../../vector3.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class ND : public Element {
		public:
			ND();

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			constexpr static const float _earthEquatorialRadius = 6378137;

			// ~100m to south from airfield center & 1000 meters above MSL
			Vector3F _cameraPosition = Vector3F(
				60.01390881058745f,
				29.70243078099165f,
				1000.0f
			);

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

			static Vector3F geographicToCartesian(float distanceFromAircraftToEarthCenter, const SinAndCos& latitude, const SinAndCos& longitude);
	};
}