#include "axisEditor.h"

#include <UI/theme.h>
#include <rc.h>

namespace pizda {
	void AxisEditorTrack::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto editor = getEditor();

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
		const auto renderPin = [this, &renderer, &bounds](int32_t x, uint16_t settingsValue, bool to) {
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
		if (event->getTypeID() == PointerDownEvent::typeID) {
			const auto editor = getEditor();
			const auto& bounds = getBounds();

			const auto pointerX = reinterpret_cast<PointerDownEvent*>(event)->getPosition().getX();
			const int32_t pointerValue = (pointerX - bounds.getX()) * Axis::maxValue / bounds.getWidth();

			_selectedPin = std::abs(pointerValue - editor->getAxis()->getSettings()->to) <= std::abs(pointerValue - editor->getAxis()->getSettings()->from) ? SelectedPin::to : SelectedPin::from;

			setCaptured(true);

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			const auto editor = getEditor();
			const auto settings = editor->getAxis()->getSettings();
			const auto pointerX = reinterpret_cast<PointerDragEvent*>(event)->getPosition().getX();

			const auto& bounds = getBounds();
			const int32_t clampedTouchX = std::clamp(pointerX - bounds.getX(), static_cast<int32_t>(0), static_cast<int32_t>(bounds.getWidth()));

			// Updating settings
			const uint16_t value = clampedTouchX * Axis::maxValue / bounds.getWidth();

			if (_selectedPin == SelectedPin::to) {
				settings->to = std::max(settings->from, value);
			}
			else {
				settings->from = std::min(settings->to, value);
			}

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			_selectedPin = SelectedPin::none;

			RC::getInstance().getSettings().axis.scheduleWrite();

			setCaptured(false);

			event->setHandled(true);
		}
	}

	AxisEditor* AxisEditorTrack::getEditor() const {
		return dynamic_cast<AxisEditor*>(getParent());
	}

	AxisEditor::AxisEditor(Axis* axis) : _axis(axis) {
		setHeight(Theme::elementHeight);

		// Invert button
		_invertButton.setWidth(Theme::elementHeight);
		_invertButton.setHorizontalAlignment(Alignment::end);

		_invertButton.setCornerRadius(Theme::cornerRadius);

		_invertButton.setDefaultBackgroundColor(&Theme::bg3);
		_invertButton.setActiveBackgroundColor(&Theme::fg1);

		_invertButton.setDefaultTextColor(&Theme::bg7);
		_invertButton.setActiveTextColor(&Theme::bg2);

		_invertButton.setFont(&Theme::fontSmall);
		_invertButton.setText(L"INV");

		_invertButton.setToggle(true);
		_invertButton.setActive(_axis->getSettings()->inverted);

		_invertButton.click += [this] {
			_axis->getSettings()->inverted = _invertButton.isActive();

			RC::getInstance().getSettings().axis.scheduleWrite();
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