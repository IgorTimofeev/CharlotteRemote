#include "axisEditor.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	void AxisEditorTrack::onRender(Renderer* renderer, const Bounds& bounds) {
		auto editor = getEditor();

		// Track
		renderer->renderFilledRectangle(bounds, Theme::cornerRadius, &Theme::bg2);
		renderer->renderRectangle(bounds, Theme::cornerRadius, &Theme::bg3);

		// Fill
		const int32_t fromX = bounds.getX() + editor->getAxis()->getSettings()->from * bounds.getWidth() / 4096;
		const int32_t toX = bounds.getX() + editor->getAxis()->getSettings()->to * bounds.getWidth() / 4096;
		const uint16_t fillWidth = toX - fromX;

		renderer->renderFilledRectangle(
			Bounds(
				fromX,
				bounds.getY(),
				fillWidth,
				bounds.getHeight()
			),
			Theme::cornerRadius,
			&Theme::bg3
		);

		// Middle line
		renderer->renderVerticalLine(
			Point(fromX + fillWidth / 2, bounds.getY()),
			bounds.getHeight(),
			&Theme::bg4
		);

		// Axis value thumb
		constexpr uint16_t axisValueThumbWidth = 2;

		renderer->renderFilledRectangle(
			Bounds(
				bounds.getX() + axisValueThumbWidth / 2 + editor->getAxis()->getRawValue() * (bounds.getWidth() - axisValueThumbWidth) / 4096,
				bounds.getY(),
				axisValueThumbWidth,
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
			constexpr uint8_t textOffsetX = 4;
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
			editor->getAxis()->getSettings()->from,
			false
		);

		renderPin(
			toX,
			editor->getAxis()->getSettings()->to,
			true
		);

		Element::onRender(renderer, bounds);
	}

	void AxisEditorTrack::onEvent(Event* event) {
		Element::onEvent(event);

		const auto isTouchDown = event->getTypeID() == TouchDownEvent::typeID;
		const auto isTouchDrag = event->getTypeID() == TouchDragEvent::typeID;
		const auto isTouchUp = event->getTypeID() == TouchUpEvent::typeID;

		if (!(isTouchDown || isTouchDrag || isTouchUp))
			return;

		if (isTouchDown) {
			const auto touchDownEvent = (TouchDownEvent*) event;

			auto editor = getEditor();
			const auto& bounds = getBounds();

			const auto touchX = touchDownEvent->getPosition().getX();
			const int32_t touchValue = (touchX - bounds.getX()) * 4096 / bounds.getWidth();

			_touchedTo = std::abs(touchValue - editor->getAxis()->getSettings()->to) <= std::abs(touchValue - editor->getAxis()->getSettings()->from);

			setCaptured(true);
		}
		else if (isTouchDrag) {
			const auto touchDragEvent = (TouchDragEvent*) event;

			auto editor = getEditor();
			const auto touchX = touchDragEvent->getPosition().getX();

			const auto& bounds = getBounds();
			const int32_t clampedTouchX = yoba::clamp((int32_t) (touchX - bounds.getX()), (int32_t) 0, (int32_t) bounds.getWidth());

			// Updating settings
			auto settingsValue =
				_touchedTo
				? &editor->getAxis()->getSettings()->to
				: &editor->getAxis()->getSettings()->from;

			*settingsValue = clampedTouchX * 4096 / bounds.getWidth();
		}
		else {
			// Saving changes
			RC::getInstance().getSettings().enqueueWrite();

			setCaptured(false);
		}

		event->setHandled(true);
	}

	AxisEditor* AxisEditorTrack::getEditor() {
		return dynamic_cast<AxisEditor*>(getParent());
	}

	void AxisEditor::setup(Axis* axis) {
		_axis = axis;

		setOrientation(Orientation::horizontal);
		setSpacing(8);
		setHeight(Theme::elementHeight);

		// Track
		*this += &_track;

		// Invert button
		_invertButton.setToggle(true);
		_invertButton.setWidth(Theme::elementHeight);
		_invertButton.setCornerRadius(Theme::cornerRadius);

		_invertButton.setDefaultBackgroundColor(&Theme::bg2);
		_invertButton.setPressedBackgroundColor(&Theme::fg1);

		_invertButton.setDefaultTextColor(&Theme::fg7);
		_invertButton.setPressedTextColor(&Theme::bg2);

		_invertButton.setFont(&Theme::fontSmall);
		_invertButton.setText(L"INV");

		_invertButton.setPressed(_axis->getSettings()->inverted);

		_invertButton.pressedChanged += [this]() {
			_axis->getSettings()->inverted = _invertButton.isPressed();

			RC::getInstance().getSettings().enqueueWrite();
		};

		setFit(&_invertButton, true);
		*this += &_invertButton;
	}

	Axis* AxisEditor::getAxis() const {
		return _axis;
	}
}