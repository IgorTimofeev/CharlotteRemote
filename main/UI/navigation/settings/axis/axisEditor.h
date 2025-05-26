#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <hardware/axis.h>

namespace pizda {
	using namespace YOBA;

	class AxisEditor;

	enum class SelectedPin : uint8_t {
		none,
		from,
		to
	};

	class AxisEditorTrack : public Element {
		public:

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onTouchDown(TouchDownEvent* event) override;
			void onTouchUp(TouchUpEvent* event) override;
			void onTouchDrag(TouchDragEvent* event) override;

		private:
			SelectedPin _selectedPin = SelectedPin::none;

			AxisEditor* getEditor() const;
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