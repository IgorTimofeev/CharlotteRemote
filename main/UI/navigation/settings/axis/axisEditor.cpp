#include "axisEditor.h"

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	void AxisEditorTrack::onEvent(Event* event) {
		if (event->getTypeID() == PointerDownEvent::typeID) {
			const auto editor = getEditor();
			const auto& bounds = getBounds();
			
			pointerDownX = reinterpret_cast<PointerDownEvent*>(event)->getPosition().getX();
			const int32_t ADCValue = (pointerDownX - bounds.getX()) * Axis::valueMax / bounds.getWidth();
			
			const auto range = editor->getAxis()->getSettings()->to - editor->getAxis()->getSettings()->from;
			const auto middle = editor->getAxis()->getSettings()->from + range / 2;
			
			const auto fromDelta = std::abs(ADCValue - editor->getAxis()->getSettings()->from);
			const auto middleDelta = std::abs(ADCValue - middle);
			const auto toDelta = std::abs(ADCValue - editor->getAxis()->getSettings()->to);
			
			// Range check comes first to prevent deadlock on edges
			if (ADCValue <= editor->getAxis()->getSettings()->from) {
				_selectedPin = SelectedPin::from;
			}
			else if (ADCValue >= editor->getAxis()->getSettings()->to) {
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

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			const auto settings = getEditor()->getAxis()->getSettings();
			const auto pointerX = reinterpret_cast<PointerDragEvent*>(event)->getPosition().getX();

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

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			_selectedPin = SelectedPin::none;
			pointerDownX = -1;
			
			setCaptured(false);
			
			RC::getInstance().getSettings().axis.scheduleWrite();
			
			event->setHandled(true);
		}
	}
	
	void AxisEditorTrack::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto editor = getEditor();
		
		// Track
		renderer->renderFilledRectangle(bounds, Theme::cornerRadius, &Theme::bg2);
		renderer->renderRectangle(bounds, Theme::cornerRadius, &Theme::bg3);
		
		// Fill
		const auto settingsDelta = editor->getAxis()->getSettings()->to - editor->getAxis()->getSettings()->from;
		const int32_t fromX = bounds.getX() + editor->getAxis()->getSettings()->from * bounds.getWidth() / Axis::valueMax;
		const int32_t toX = bounds.getX() + editor->getAxis()->getSettings()->to * bounds.getWidth() / Axis::valueMax;
		const uint16_t fillWidth = toX - fromX + 1;
		
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
		
		// Curve
		Point curvePos0 {};
		
		for (int32_t i = 0; i < fillWidth; ++i) {
			const auto ADCValue =
				editor->getAxis()->getSettings()->from
				+ settingsDelta * i / fillWidth;
			
			Point curvePos1 {
				fromX + i,
				bounds.getY2() - bounds.getHeight() * editor->getAxis()->mapValue(ADCValue) / Axis::valueMax
			};
			
			if (i > 0) {
				renderer->renderLine(
					curvePos0,
					curvePos1,
					_selectedPin == SelectedPin::middle ? &Theme::fg1 : &Theme::bg4
				);
			}
			
			curvePos0 = curvePos1;
		}
		
		// Middle line
		renderer->renderVerticalLine(
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
			renderer->renderVerticalLine(
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
			
			renderer->renderFilledRectangle(flagBounds, bg);
			
			renderer->renderString(
				Point(flagBounds.getX() + textOffsetX, flagBounds.getY() + textOffsetY),
				&Theme::fontSmall,
				fg,
				text
			);
		};
		
		renderSidePin(
			fromX,
			editor->getAxis()->getSettings()->from,
			false
		);
		
		renderSidePin(
			toX,
			editor->getAxis()->getSettings()->to,
			true
		);
		
		// Thumb
		const auto thumbX = bounds.getX() + editor->getAxis()->getRawValue() * bounds.getWidth() / Axis::valueMax;
		const auto thumbInWorkingRange = thumbX >= fromX && thumbX <= toX;
		
		renderer->renderVerticalLine(
			Point(
				thumbX,
				bounds.getY()
			),
			bounds.getHeight(),
			thumbInWorkingRange ? &Theme::accent1 : &Theme::bad1
		);
		
		if (thumbInWorkingRange) {
			const auto oldViewport = renderer->pushViewport(Bounds(
				fromX,
				bounds.getY(),
				fillWidth,
				bounds.getHeight()
			));
			
			renderer->renderFilledCircle(
				Point(
					thumbX,
					bounds.getY2() - bounds.getHeight() * editor->getAxis()->mapValue(editor->getAxis()->getRawValue()) / Axis::valueMax
				),
				2,
				&Theme::fg1
			);
			
			renderer->popViewport(oldViewport);
		}
		
		
		Element::onRender(renderer, bounds);
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
		_invertButton.setContentMargin(Margin(_invertButton.getCornerRadius(), 0, 0, 0));

		_invertButton.setDefaultBackgroundColor(&Theme::bg3);
		_invertButton.setActiveBackgroundColor(&Theme::fg1);

		_invertButton.setDefaultTextColor(&Theme::bg7);
		_invertButton.setActiveTextColor(&Theme::bg2);
		
		_invertButton.setFont(&Theme::fontSmall);
		_invertButton.setText(L"<->");

		_invertButton.setToggle(true);
		_invertButton.setActive(_axis->getSettings()->invertOutput);

		_invertButton.click += [this] {
			_axis->getSettings()->invertOutput = _invertButton.isActive();

			RC::getInstance().getSettings().axis.scheduleWrite();
		};

		*this += &_invertButton;

		// Track
		_track.setMargin(Margin(0, 0, _invertButton.getSize().getWidth() - _invertButton.getCornerRadius() - 1, 0));
		*this += &_track;
	}

	Axis* AxisEditor::getAxis() const {
		return _axis;
	}
}