#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../../vector3.h"
#include "object.h"
#include "camera.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class SpatialView : public Element {
		public:
			Camera& getCamera();

			void addObject(SpatialObject* object);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			Camera _camera;
			std::vector<SpatialObject*> _objects;
	};
}