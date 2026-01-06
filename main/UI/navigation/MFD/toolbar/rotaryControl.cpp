#include "rotaryControl.h"

#include "rc.h"

namespace pizda {
	
	RotaryControl::RotaryControl() {
		*this += &variantsLayout;
		
		setDefaultMargin(&variantsLayout);
	}
	
	uint8_t RotaryControl::getVariantIndex() const {
		return variantIndex;
	}
	
	bool RotaryControl::isVariantSelectMode() const {
		return variantSelectMode;
	}
	
	void RotaryControl::onEventBeforeChildren(Event* event) {
		if (event->getTypeID() == PointerDownEvent::typeID) {
			wasFocusedOnDown = isFocused();
		}
		
		ToolbarSection::onEventBeforeChildren(event);
		
		if (!isFocused())
			return;
		
		if (event->getTypeID() == PointerDownEvent::typeID || event->getTypeID() == PushButtonEncoderDownEvent::typeID) {
			if (event->getTypeID() == PointerDownEvent::typeID) {
				auto downEvent = reinterpret_cast<PointerDownEvent*>(event);
				pressX = downEvent->getPosition().getX();
				
				setCaptured(true);
			}
			
			pressTime = esp_timer_get_time();
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID || event->getTypeID() == PushButtonEncoderUpEvent::typeID) {
			if (event->getTypeID() == PointerUpEvent::typeID) {
				setCaptured(false);
			}
			
			pressTime = 0;
			pressX = -1;
			
			if (event->getTypeID() != PointerUpEvent::typeID || wasFocusedOnDown) {
				if (preventModeSwitch) {
					preventModeSwitch = false;
				}
				else {
					if (variantSelectMode) {
						variantSelectMode = false;
						
						RC::getInstance().getAudioPlayer().playFeedback();
					}
					else {
						if (variantsLayout.getChildrenCount() > 1) {
							variantSelectMode = true;
							
							RC::getInstance().getAudioPlayer().playFeedback();
						}
					}
				}
			}
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			if (pressX >= 0 && wasFocusedOnDown) {
				const auto dragX = reinterpret_cast<PointerDragEvent*>(event)->getPosition().getX();
				const auto deltaX = dragX - pressX;
				
				const uint8_t threshold = variantSelectMode ? 20 : 5;
				
				if (std::abs(deltaX) > threshold) {
					pressTime = 0;
					pressX = dragX;
					preventModeSwitch = true;
					
					onRotateOrDrag(deltaX > 0, std::abs(deltaX) > 20);
				}
			}
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
			const auto dps = static_cast<EncoderValueChangedEvent*>(event)->getDPS();
			
			onRotateOrDrag(dps > 0, std::abs(dps) > 80);
			
			event->setHandled(true);
		}
	}
	
	void RotaryControl::onFocusChanged() {
		ToolbarSection::onFocusChanged();
		
		if (!isFocused()) {
			variantSelectMode = false;
			pressTime = 0;
			preventModeSwitch = false;
		}
	}
	
	void RotaryControl::onTick() {
		Layout::onTick();
		
		if (pressTime == 0 || esp_timer_get_time() - pressTime < 700'000)
			return;
		
		pressTime = 0;
		preventModeSwitch = true;
		
		onLongPress();
		
		RC::getInstance().getAudioPlayer().playFeedback();
	}
	
	void RotaryControl::onRender(Renderer* renderer, const Bounds& bounds) {
		ToolbarSection::onRender(renderer, bounds);
		
		if (!variantSelectMode)
			return;
		
		const auto y = bounds.getYCenter() + 4;
		renderer->renderPixel(Point(bounds.getX() + 2, y), &Theme::fg1);
		renderer->renderPixel(Point(bounds.getX2() - 2, y), &Theme::fg1);
	}
	
	void RotaryControl::onLongPress() {
	
	}
	
	void RotaryControl::onVariantChanged() {
	
	}
	
	void RotaryControl::onRotate(bool clockwise, bool big) {
	
	}
	
	void RotaryControl::updateVariantsVisibility() {
		for (uint8_t i = 0; i < variantsLayout.getChildrenCount(); ++i) {
			variantsLayout[i]->setVisible(variantIndex == i);
		}
	}
	
	void RotaryControl::setVariantIndex(uint8_t value) {
		if (value == variantIndex)
			return;
		
		variantIndex = value;
		
		updateVariantsVisibility();
		setTitle(variantIndexToTitle(variantIndex));
	}
	
	void RotaryControl::setVariants(std::initializer_list<Element*> elements) {
		for (auto element : elements) {
			variantsLayout += element;
		}
		
		setVariantIndex(0);
	}
	
	void RotaryControl::onRotateOrDrag(bool clockwise, bool big) {
		if (variantSelectMode) {
			const auto newIndex = std::clamp<int16_t>(static_cast<int16_t>(variantIndex) + (clockwise ? 1 : -1), 0, variantsLayout.getChildrenCount() - 1);
			
			setVariantIndex(static_cast<uint8_t>(newIndex));
			
			onVariantChanged();
			
			RC::getInstance().getAudioPlayer().playFeedback();
		}
		else if (isVariantEditable(variantIndex)) {
			onRotate(clockwise, big);

			RC::getInstance().getAudioPlayer().playFeedback();
		}
	}
}