#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../hardware/axis.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AxisEditor;

	class AxisEditorPin : public Element {
		public:
			AxisEditorPin(bool isTo);

		protected:

			void onEvent(Event* event) override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;


		private:
			int32_t _oldTouchX = 0;

			bool _isTo;

			AxisEditor* getEditor();
	};

	class AxisEditor : public Layout {
		public:
			explicit AxisEditor();

			Axis* getAxis() const;

			void setAxis(Axis* axis);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			Axis* _axis = nullptr;
			AxisEditorPin _fromPin = AxisEditorPin(false);
			AxisEditorPin _toPin = AxisEditorPin(true);
	};
}