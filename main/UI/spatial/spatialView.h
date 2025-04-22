#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "element.h"
#include "camera.h"

namespace pizda {
	using namespace YOBA;

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