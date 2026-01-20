#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "hardware/axes/axis.h"

namespace pizda {
	using namespace YOBA;

	class AxisEditor;

	enum class SelectedPin : uint8_t {
		none,
		from,
		middle,
		to
	};

	class AxisEditorTrack : public Control {
		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

		private:
			SelectedPin _selectedPin = SelectedPin::none;
			int32_t pointerDownX = -1;

			AxisEditor* getEditor() const;
	};

	class AxisEditor : public Layout {
		public:
			AxisEditor(Axis* axis);

			Axis* getAxis() const;

		private:
			Axis* _axis = nullptr;

			AxisEditorTrack _track;
			Button _invertButton;
	};
}