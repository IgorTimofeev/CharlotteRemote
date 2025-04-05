#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../../theme.h"
#include "../../../../../vector3.h"
#include "../../../../../sinAndCos.h"
#include "../../../../../geographicCoordinates.h"

#include "../../../../spatial/spatialView.h"
#include "../../../../spatial/camera.h"
#include "../../../../spatial/object.h"
#include "../../../../../vector3.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class ND : public SpatialView {
		public:
			ND();

			constexpr static const uint32_t cameraOffsetMinimum = 100;
			constexpr static const uint32_t cameraOffsetMaximum = GeographicCoordinates::equatorialRadiusMeters;

			const GeographicCoordinates& getCameraOffset() const;
			void setCameraOffset(const GeographicCoordinates& value);

		protected:
			void onTick() override;

			void onEvent(Event* event) override;

		private:
			int32_t _pinchLength = 0;
			Point _touchDownPosition;

			GeographicCoordinates _cameraOffset {
				0,
				0,
				500
			};

			Label _planeObject = Label(
				GeographicCoordinates(
					yoba::toRadians(60.01449883137194),
					yoba::toRadians(29.702554658332105),
					1000
				)
				.toCartesian(),
				&Theme::fontNormal,
				&Theme::yellow,
				L"Me"
			);
	};
}