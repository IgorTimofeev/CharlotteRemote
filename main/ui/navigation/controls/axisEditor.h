#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../hardware/axis.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AxisEditor : public Element {
		public:
			AxisEditor();

			Axis* getAxis() const;

			void setAxis(Axis* axis);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

		private:
			Axis* _axis = nullptr;

			bool _touchedTo = false;
	};
}