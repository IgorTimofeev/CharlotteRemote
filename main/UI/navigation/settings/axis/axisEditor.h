#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "systems/axes/axis.h"

namespace pizda {
	using namespace YOBA;

	class AxisEditor;

	enum class SelectedPin : uint8_t {
		none,
		from,
		middle,
		to
	};

	class AxisEditor : public Control {
		public:
			AxisEditor(Axis* axis);
			Axis* getAxis() const;

		protected:
			void onRender(Renderer& renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

		private:
			Axis* _axis;

			SelectedPin _selectedPin = SelectedPin::none;
			int32_t pointerDownX = -1;
	};
}