#include "axisEditor.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	AxisEditor::AxisEditor() {
		setHeight(30);
	}

	void AxisEditor::onRender(Renderer* renderer, const Bounds& bounds) {
		// Track
		renderer->renderFilledRectangle(bounds, Theme::cornerRadius, &Theme::bg2);
		renderer->renderRectangle(bounds, Theme::cornerRadius, &Theme::bg3);

		// Fill
		const int32_t fromX = bounds.getX() + _axis->getSettings()->from * bounds.getWidth() / 4096;
		const int32_t toX = bounds.getX() + _axis->getSettings()->to * bounds.getWidth() / 4096;

		renderer->renderFilledRectangle(
			Bounds(
				fromX,
				bounds.getY(),
				toX - fromX,
				bounds.getHeight()
			),
			Theme::cornerRadius,
			&Theme::bg4
		);

		// Axis value thumb
		constexpr uint16_t axisValueBarWidth = 4;

		renderer->renderFilledRectangle(
			Bounds(
				bounds.getX() + _axis->getRawValue() * (bounds.getWidth() - axisValueBarWidth) / 4096,
				bounds.getY(),
				axisValueBarWidth,
				bounds.getHeight()
			),
			&Theme::accent1
		);

		// Pins
		const auto renderPin = [renderer, &bounds](int32_t x, uint16_t settingsValue, bool to) {
			// Line
			renderer->renderVerticalLine(
				Point(x, bounds.getY()),
				bounds.getHeight() - Theme::cornerRadius,
				&Theme::fg1
			);

			// Flag
			const auto text = std::to_wstring(settingsValue * 100 / 4096);
			constexpr uint8_t textOffsetX = 5;
			constexpr uint8_t textOffsetY = 0;

			const auto flagSize = Size(
				Theme::fontSmall.getWidth(text) + textOffsetX * 2,
				Theme::fontSmall.getHeight() + textOffsetY * 2
			);

			const auto flagBounds = Bounds(
				Point(
					to ? x - flagSize.getWidth() : x,
					bounds.getY() + bounds.getHeight() - flagSize.getHeight()
				),
				flagSize
			);

			renderer->renderFilledRectangle(flagBounds, Theme::cornerRadius, &Theme::fg1);

			renderer->renderString(
				Point(flagBounds.getX() + textOffsetX, flagBounds.getY() + textOffsetY),
				&Theme::fontSmall,
				&Theme::bg1,
				text
			);
		};

		renderPin(
			fromX,
			_axis->getSettings()->from,
			false
		);

		renderPin(
			toX,
			_axis->getSettings()->to,
			true
		);

		Element::onRender(renderer, bounds);
	}

	Axis* AxisEditor::getAxis() const {
		return _axis;
	}

	void AxisEditor::setAxis(Axis* axis) {
		_axis = axis;
	}

	void AxisEditor::onEvent(Event* event) {
		Element::onEvent(event);

		const auto isTouchDown = event->getTypeID() == TouchDownEvent::typeID;
		const auto isTouchDrag = event->getTypeID() == TouchDragEvent::typeID;
		const auto isTouchUp = event->getTypeID() == TouchUpEvent::typeID;

		if (!(isTouchDown || isTouchDrag || isTouchUp))
			return;

		if (isTouchDown) {
			const auto touchDownEvent = (TouchDownEvent*) event;

			const auto& bounds = getBounds();

			const auto touchX = touchDownEvent->getPosition().getX();
			const int32_t touchValue = (touchX - bounds.getX()) * 4096 / bounds.getWidth();

			_touchedTo = std::abs(touchValue - _axis->getSettings()->to) <= std::abs(touchValue - _axis->getSettings()->from);

			setCaptured(true);
		}
		else if (isTouchDrag) {
			const auto touchDragEvent = (TouchDragEvent*) event;

			const auto touchX = touchDragEvent->getPosition().getX();

			const auto& bounds = getBounds();
			const int32_t clampedTouchX = yoba::clamp((int32_t) (touchX - bounds.getX()), (int32_t) 0, (int32_t) bounds.getWidth());

			// Updating settings
			auto settingsValue = _touchedTo ? &_axis->getSettings()->to : &_axis->getSettings()->from;
			*settingsValue = clampedTouchX * 4096 / bounds.getWidth();
		}
		else {
			// Saving changes
			RC::getInstance().getSettings().enqueueWrite();

			setCaptured(false);
		}

		event->setHandled(true);
	}
}