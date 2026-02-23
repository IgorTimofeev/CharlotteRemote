#include "axisEditor.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	AxisEditor::AxisEditor(Axis* axis) : _axis(axis) {
		setHeight(Theme::elementHeight);
	}
	
	void AxisEditor::onEvent(Event& event) {
		if (event.getTypeID() == PointerDownEvent::typeID) {
			const auto& bounds = getBounds();
			
			pointerDownX = reinterpret_cast<PointerDownEvent&>(event).getPosition().getX();
			const int32_t ADCValue = (pointerDownX - bounds.getX()) * Axis::valueMax / bounds.getWidth();
			
			const auto range = _axis->getSettings()->to - _axis->getSettings()->from;
			const auto middle = _axis->getSettings()->from + range / 2;
			
			const auto fromDelta = std::abs(ADCValue - _axis->getSettings()->from);
			const auto middleDelta = std::abs(ADCValue - middle);
			const auto toDelta = std::abs(ADCValue - _axis->getSettings()->to);
			
			// Range check comes first to prevent deadlock on edges
			if (ADCValue <= _axis->getSettings()->from) {
				_selectedPin = SelectedPin::from;
			}
			else if (ADCValue >= _axis->getSettings()->to) {
				_selectedPin = SelectedPin::to;
			}
			// Normal case
			else {
				_selectedPin =
					fromDelta < toDelta
					? (
						fromDelta < middleDelta
						? SelectedPin::from
						: SelectedPin::middle
					)
					: (
						toDelta < middleDelta
						? SelectedPin::to
						: SelectedPin::middle
					);
			}
			
			setCaptured(true);

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerDragEvent::typeID) {
			const auto settings = _axis->getSettings();
			const auto pointerX = reinterpret_cast<PointerDragEvent&>(event).getPosition().getX();

			// Updating settings
			if (_selectedPin == SelectedPin::middle) {
				const auto deltaX = pointerX - pointerDownX;
				pointerDownX = pointerX;
				
				settings->sensitivity = static_cast<uint8_t>(std::clamp<int16_t>(static_cast<int16_t>(settings->sensitivity) + deltaX * 0xFF / 100, 0x00, 0xFF));
			}
			else {
				const auto& bounds = getBounds();
				
				const auto posClamped = std::clamp<int32_t>(pointerX - bounds.getX(), 0, bounds.getWidth());
				const uint16_t ADCValue = posClamped * Axis::valueMax / bounds.getWidth();
				
				if (_selectedPin == SelectedPin::from) {
					settings->from = std::min(ADCValue, settings->to);
				}
				else {
					settings->to = std::max(ADCValue, settings->from);
				}
			}

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerUpEvent::typeID) {
			_selectedPin = SelectedPin::none;
			pointerDownX = -1;
			
			setCaptured(false);
			
			RC::getInstance().getSettings().axes.scheduleWrite();
			
			event.setHandled(true);
		}
	}
	
	void AxisEditor::onRender(Renderer& renderer, const Bounds& bounds) {
		// Track
		renderer.renderFilledRectangle(bounds, Theme::cornerRadius, &Theme::bg2);
		renderer.renderRectangle(bounds, Theme::cornerRadius, &Theme::bg3);
		
		// Fill
		const auto settingsDelta = _axis->getSettings()->to - _axis->getSettings()->from;
		const int32_t fromX = bounds.getX() + _axis->getSettings()->from * bounds.getWidth() / Axis::valueMax;
		const int32_t toX = bounds.getX() + _axis->getSettings()->to * bounds.getWidth() / Axis::valueMax;
		const uint16_t fillWidth = toX - fromX + 1;
		
		renderer.renderFilledRectangle(
			Bounds(
				fromX,
				bounds.getY(),
				fillWidth,
				bounds.getHeight()
			),
			Theme::cornerRadius,
			&Theme::bg3
		);
		
		// Curve
		Point curvePos0 {};
		
		for (int32_t i = 0; i < fillWidth; ++i) {
			const auto ADCValue =
				_axis->getSettings()->from
				+ settingsDelta * i / fillWidth;
			
			Point curvePos1 {
				fromX + i,
				bounds.getY2() - bounds.getHeight() * _axis->applySensitivityFilter(ADCValue) / Axis::valueMax
			};
			
			if (i > 0) {
				renderer.renderLine(
					curvePos0,
					curvePos1,
					_selectedPin == SelectedPin::middle ? &Theme::fg1 : &Theme::bg4
				);
			}
			
			curvePos0 = curvePos1;
		}
		
		// Middle line
		renderer.renderVerticalLine(
			Point(fromX + fillWidth / 2, bounds.getY() + 1),
			bounds.getHeight() - 1,
			&Theme::accent2
		);
		
		// Side pins
		const auto renderSidePin = [this, &renderer, &bounds](int32_t x, uint16_t settingsValue, bool to) {
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
			renderer.renderVerticalLine(
				Point(x, bounds.getY()),
				bounds.getHeight(),
				bg
			);
			
			// Flag
			const auto text = std::to_wstring(settingsValue * 100 / Axis::valueMax);
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
			
			renderer.renderFilledRectangle(flagBounds, bg);
			
			renderer.renderText(
				Point(flagBounds.getX() + textOffsetX, flagBounds.getY() + textOffsetY),
				Theme::fontSmall,
				fg,
				text
			);
		};
		
		renderSidePin(
			fromX,
			_axis->getSettings()->from,
			false
		);
		
		renderSidePin(
			toX,
			_axis->getSettings()->to,
			true
		);
		
		// Thumb
		const auto thumbX = bounds.getX() + _axis->getRawValue() * bounds.getWidth() / Axis::valueMax;
		const auto thumbInWorkingRange = thumbX >= fromX && thumbX <= toX;
		
		renderer.renderVerticalLine(
			Point(
				thumbX,
				bounds.getY()
			),
			bounds.getHeight(),
			thumbInWorkingRange ? &Theme::accent1 : &Theme::bad1
		);
		
		if (thumbInWorkingRange) {
			const auto oldViewport = renderer.pushViewport(Bounds(
				fromX,
				bounds.getY(),
				fillWidth,
				bounds.getHeight()
			));
			
			renderer.renderFilledCircle(
				Point(
					thumbX,
					bounds.getY2() - bounds.getHeight() * _axis->applySensitivityFilter(_axis->getRawValue()) / Axis::valueMax
				),
				2,
				&Theme::fg1
			);
			
			renderer.popViewport(oldViewport);
		}
		
		Element::onRender(renderer, bounds);
	}
	
	Axis* AxisEditor::getAxis() const {
		return _axis;
	}
}