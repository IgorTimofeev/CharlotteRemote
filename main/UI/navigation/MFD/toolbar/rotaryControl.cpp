#include "rotaryControl.h"

#include "rc.h"
#include "resources/sounds.h"

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
			wasFocusedOnLastDown = isFocused();
		}
		
		ToolbarSection::onEventBeforeChildren(event);
		
		if (!isFocused())
			return;
		
		if (event->getTypeID() == PointerDownEvent::typeID) {
			if (wasFocusedOnLastDown) {
				pressPos = reinterpret_cast<PointerDownEvent*>(event)->getPosition();
				
				setCaptured(true);
				onAnyDown();
			}
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == PushButtonEncoderDownEvent::typeID) {
			onAnyDown();
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			if (wasFocusedOnLastDown && pressPos.getX() >= 0) {
				const auto dragPos = reinterpret_cast<PointerDragEvent*>(event)->getPosition();
				const auto deltaPos = dragPos - pressPos;
				
				const uint8_t threshold = variantSelectMode ? 20 : 5;
				
				// Resetting long press on any direction
				if (deltaPos.getLength() >= threshold) {
					longPressTime = 0;
					callOnPress = false;
				}
				
				// Rotating only on X
				if (std::abs(deltaPos.getX()) >= threshold) {
					pressPos = dragPos;
					
					onAnyRotate(deltaPos.getX() > 0, std::abs(deltaPos.getX()) > 10);
				}
			}
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == EncoderValueChangedEvent::typeID) {
			const auto dps = static_cast<EncoderValueChangedEvent*>(event)->getDPS();
			
			onAnyRotate(dps > 0, std::abs(dps) > 80);
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			if (wasFocusedOnLastDown) {
				setCaptured(false);
				
				pressPos = { -1, -1 };
				
				onAnyUp();
			}
			
			event->setHandled(true);
		}
		else if (event->getTypeID() == PushButtonEncoderUpEvent::typeID) {
			onAnyUp();
			
			event->setHandled(true);
		}
	}
	
	void RotaryControl::onFocusChanged() {
		ToolbarSection::onFocusChanged();
		
		if (isFocused())
			return;
		
		pressPos = { -1, -1 };
		longPressTime = 0;
		callOnPress = true;
	}
	
	void RotaryControl::onTick() {
		Layout::onTick();
		
		if (longPressTime == 0 || esp_timer_get_time() - longPressTime < 700'000)
			return;
		
		longPressTime = 0;
		callOnPress = false;
		variantSelectMode = !variantSelectMode;
		
		if (variantSelectMode) {
			RC::getInstance().getAudioPlayer().playFeedback(resources::sounds::engaged);
		}
		else {
			RC::getInstance().getAudioPlayer().playFeedback(resources::sounds::disengaged);
		}
	}
	
	void RotaryControl::onRender(Renderer* renderer, const Bounds& bounds) {
		ToolbarSection::onRender(renderer, bounds);
		
		if (!variantSelectMode)
			return;
		
		const auto dotWidth = 1;
		constexpr static uint8_t dotSpacing = 3;
		const auto dotsWidth = variantsLayout.getChildrenCount() * (dotWidth + dotSpacing) - dotSpacing;
		
		auto x = bounds.getXCenter() - dotsWidth / 2;
		const auto y = bounds.getY2() - 4;
		
		for (uint8_t i = 0; i < variantsLayout.getChildrenCount(); ++i) {
			renderer->renderPixel(Point(x, y), variantIndex == i ? &Theme::fg1 : &Theme::fg3);
			
			x += dotWidth + dotSpacing;
		}
		
//		constexpr static uint8_t offset = 1;
//
//		const auto y = bounds.getYCenter() + 4;
//		auto x = bounds.getX() + offset;
//
//		renderer->renderPixel(Point(x + 1, y - 1), &Theme::fg1);
//		renderer->renderPixel(Point(x, y), &Theme::fg1);
//		renderer->renderPixel(Point(x + 1, y + 1), &Theme::fg1);
//
//		x = bounds.getX2() - offset;
//		renderer->renderPixel(Point(x - 1, y - 1), &Theme::fg1);
//		renderer->renderPixel(Point(x, y), &Theme::fg1);
//		renderer->renderPixel(Point(x - 1, y + 1), &Theme::fg1);
	}
	
	void RotaryControl::onVariantChanged() {
	
	}
	
	void RotaryControl::onRotate(bool clockwise, bool big) {
	
	}
	
	void RotaryControl::onPress() {
	
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
	
	void RotaryControl::onAnyDown() {
		callOnPress = true;
		
		if (variantsLayout.getChildrenCount() > 1)
			longPressTime = esp_timer_get_time();
	}
	
	void RotaryControl::onAnyUp() {
		longPressTime = 0;
		
		if (callOnPress) {
			variantSelectMode = false;
			
			RC::getInstance().getAudioPlayer().playFeedback();
			
			onPress();
		}
	}
	
	void RotaryControl::onAnyRotate(bool clockwise, bool big) {
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