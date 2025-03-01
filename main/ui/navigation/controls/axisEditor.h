#pragma once

#include "src/main.h"
#include "src/ui.h"

#include "../../../hardware/axis.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AxisEditor;

	class AxisEditorTrack : public Element {
		public:

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

		private:
			bool _touchedTo = false;

			AxisEditor* getEditor();
	};

	class AxisEditor : public Layout {
		public:
			explicit AxisEditor(Axis* axis);

			Axis* getAxis() const;

		private:
			Axis* _axis = nullptr;

			AxisEditorTrack _track;
			Button _invertButton;
	};
}