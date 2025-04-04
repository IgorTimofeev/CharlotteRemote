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

			const Vector3F& getCameraOffset() const;
			void setCameraOffset(const Vector3F& value);

		protected:
			void onTick() override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event* event) override;

		private:
			int32_t _pinchLength = 0;
			Point _touchDownPosition;

			Vector3F _cameraOffset {
				0,
				0,
				100
			};
	};
}