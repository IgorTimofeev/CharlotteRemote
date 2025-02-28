#include "axisEditor.h"

#include "../../theme.h"

namespace pizda {
	AxisEditorPin::AxisEditorPin(bool isTo) : _isTo(isTo) {
		setWidth(30);
	}

	void AxisEditorPin::onRender(Renderer* renderer, const Bounds& bounds) {
		Element::onRender(renderer, bounds);

		const uint8_t circleRadius = 10;
		const uint16_t lineHeight = bounds.getHeight() - circleRadius * 2;

		// Line
		renderer->renderVerticalLine(bounds.getTopLeft(), lineHeight, &Theme::fg1);

		// Circle
		const auto circleCenter = Point(bounds.getX(), bounds.getY() + lineHeight + circleRadius);
		renderer->renderFilledCircle(circleCenter, circleRadius, &Theme::fg1);

		// Text
		const auto text = _isTo ?  L"<" : L">";

		renderer->renderString(
			Point(circleCenter.getX() - Theme::fontSmall.getWidth(text) / 2, circleCenter.getY() - Theme::fontSmall.getHeight() / 2),
			&Theme::fontSmall,
			&Theme::bg1,
			text
		);
	}

	void AxisEditorPin::onEvent(Event* event) {
		Element::onEvent(event);

		const auto isTouchDown = event->getTypeID() == TouchDownEvent::typeID;
		const auto isTouchDrag = event->getTypeID() == TouchDragEvent::typeID;
		const auto isTouchUp = event->getTypeID() == TouchUpEvent::typeID;

		if (!(isTouchDown || isTouchDrag || isTouchUp))
			return;

		if (isTouchDown) {
			auto touchDownEvent = (TouchDownEvent*) event;

			_oldTouchX = touchDownEvent->getPosition().getX();

			setCaptured(true);
		}
		else if (isTouchDrag) {
			auto touchDragEvent = (TouchDragEvent*) event;

			auto touchX = touchDragEvent->getPosition().getX();
			auto deltaX = touchX - _oldTouchX;
			_oldTouchX = touchX;

			auto editor = getEditor();
			const auto& editorBounds = editor->getBounds();

			// Updating margin
			auto marginLeft = yoba::clamp(getMargin().getLeft() + deltaX, (int32_t) 0, (int32_t) editorBounds.getWidth());
			setMargin(Margin(marginLeft, 0, 0, 0));

			// Updating settings
			auto axis = editor->getAxis();
			auto settingsValue = _isTo ? &axis->getSettings()->to : &axis->getSettings()->from;
			*settingsValue = marginLeft * 4096 / (int32_t) editorBounds.getWidth();
		}
		else {
			_oldTouchX = 0;

			setCaptured(false);
		}

		event->setHandled(true);
	}

	AxisEditor::AxisEditor() {
		setHeight(30);

		_fromPin.setHorizontalAlignment(Alignment::start);
		*this += &_fromPin;

		_toPin.setHorizontalAlignment(Alignment::start);
		*this += &_toPin;
	}

	AxisEditor* AxisEditorPin::getEditor() {
		return dynamic_cast<AxisEditor*>(getParent());
	}

	void AxisEditor::onRender(Renderer* renderer, const Bounds& bounds) {
		const uint16_t barHeight = 24;

		renderer->renderFilledRectangle(Bounds(bounds.getX(), bounds.getY(), bounds.getWidth(), barHeight), Theme::cornerRadius, &Theme::bg4);

		const int32_t axisX = _axis->getProcessedValue() * bounds.getWidth() / 4096;
		const uint16_t axisWidth = 10;

		renderer->renderFilledRectangle(Bounds(bounds.getX() + axisX, bounds.getY(), axisWidth, barHeight), Theme::cornerRadius, &Theme::accent1);

		Layout::onRender(renderer, bounds);
	}

	Axis* AxisEditor::getAxis() const {
		return _axis;
	}

	void AxisEditor::setAxis(Axis* axis) {
		_axis = axis;
	}
}