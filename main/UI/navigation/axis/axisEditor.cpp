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
		const int32_t fromX = bounds.getX() + editor->getAxis()->getSettings()->from * bounds.getWidth() / Axis::maxValue;
		const int32_t toX = bounds.getX() + editor->getAxis()->getSettings()->to * bounds.getWidth() / Axis::maxValue;
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
			&Theme::bg6
		);

		// Axis value thumb
		constexpr uint16_t axisValueThumbWidth = 2;

		renderer->renderFilledRectangle(
			Bounds(
				bounds.getX() + axisValueThumbWidth / 2 + editor->getAxis()->getRawValue() * (bounds.getWidth() - axisValueThumbWidth) / Axis::maxValue,
				bounds.getY(),
				axisValueThumbWidth,
				bounds.getHeight()
			),
			&Theme::accent1
		);

		// Pins
		const auto renderPin = [this, renderer, &bounds](int32_t x, uint16_t settingsValue, bool to) {
			const Color* bg;
			const Color* fg;

			if ((_selectedPin == SelectedPin::to && to) || (_selectedPin == SelectedPin::from && !to)) {
				bg = &Theme::fg1;
				fg = &Theme::bg1;
			}
			else {
				bg = &Theme::bg6;
				fg = &Theme::fg1;
			}

			// Line
			renderer->renderVerticalLine(
				Point(x, bounds.getY()),
				bounds.getHeight() - Theme::cornerRadius,
				bg
			);

			// Flag
			const auto text = std::to_wstring(settingsValue * 100 / Axis::maxValue);
			constexpr uint8_t textOffsetX = 4;
			constexpr uint8_t textOffsetY = 1;

			const auto flagSize = Size(
				Theme::fontSmall.getWidth(text) + textOffsetX * 2,
				Theme::fontSmall.getHeight() + textOffsetY * 2
			);

			const auto flagBounds = Bounds(
				Point(
					to ? x - flagSize.getWidth() + 1 : x,
					bounds.getY() + bounds.getHeight() - flagSize.getHeight()
				),
				flagSize
			);

			renderer->renderFilledRectangle(flagBounds, bg);

			renderer->renderString(
				Point(flagBounds.getX() + textOffsetX, flagBounds.getY() + textOffsetY),
				&Theme::fontSmall,
				fg,
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
			const int32_t touchValue = (touchX - bounds.getX()) * Axis::maxValue / bounds.getWidth();

			_selectedPin = std::abs(touchValue - editor->getAxis()->getSettings()->to) <= std::abs(touchValue - editor->getAxis()->getSettings()->from) ? SelectedPin::to : SelectedPin::from;

			setCaptured(true);
		}
		else if (isTouchDrag) {
			const auto touchDragEvent = (TouchDragEvent*) event;

			auto editor = getEditor();
			auto settings = editor->getAxis()->getSettings();
			const auto touchX = touchDragEvent->getPosition().getX();

			const auto& bounds = getBounds();
			const int32_t clampedTouchX = std::clamp((int32_t) (touchX - bounds.getX()), (int32_t) 0, (int32_t) bounds.getWidth());

			// Updating settings
			uint16_t value = clampedTouchX * Axis::maxValue / bounds.getWidth();

			if (_selectedPin == SelectedPin::to) {
				settings->to = std::max(settings->from, value);
			}
			else {
				settings->from = std::min(settings->to, value);
			}
		}
		else {
			_selectedPin = SelectedPin::none;

			// Saving changes
			RC::getInstance().getSettings().enqueueWrite();

			setCaptured(false);
		}

		event->setHandled(true);
	}

	AxisEditor* AxisEditorTrack::getEditor() {
		return dynamic_cast<AxisEditor*>(getParent());
	}

	AxisEditor::AxisEditor(Axis* axis) : _axis(axis) {
		setHeight(Theme::elementHeight);

		// Invert button
		_invertButton.setWidth(Theme::elementHeight);
		_invertButton.setHorizontalAlignment(Alignment::end);

		_invertButton.setCheckMode(ButtonCheckMode::toggle);
		_invertButton.setCornerRadius(Theme::cornerRadius);

		_invertButton.setDefaultBackgroundColor(&Theme::bg3);
		_invertButton.setPressedBackgroundColor(&Theme::fg1);

		_invertButton.setDefaultTextColor(&Theme::bg7);
		_invertButton.setPressedTextColor(&Theme::bg2);

		_invertButton.setFont(&Theme::fontSmall);
		_invertButton.setText(L"INV");

		_invertButton.setCheckMode(ButtonCheckMode::toggle);
		_invertButton.setChecked(_axis->getSettings()->inverted);

		_invertButton.click += [this]() {
			_axis->getSettings()->inverted = _invertButton.isChecked();

			RC::getInstance().getSettings().enqueueWrite();
		};

		*this += &_invertButton;

		// Track
		_track.setMargin(Margin(0, 0, Theme::elementHeight - Theme::cornerRadius - 1, 0));
		*this += &_track;
	}

	Axis* AxisEditor::getAxis() const {
		return _axis;
	}
}