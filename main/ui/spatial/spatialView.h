#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../../vector3.h"
#include "element.h"
#include "camera.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class SpatialView : public virtual Element {
		public:
			Camera& getCamera();
			void addElement(SpatialElement* element);
			void operator+=(SpatialElement* element);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			Camera _camera;
			std::vector<SpatialElement*> _objects;
	};
}