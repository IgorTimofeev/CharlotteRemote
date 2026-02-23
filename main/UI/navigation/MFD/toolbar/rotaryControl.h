#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include <YOBA/hardware/encoder.h>

#include <esp_log.h>
#include <esp_timer.h>

#include "UI/theme.h"
#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "UI/navigation/MFD/toolbar/toolbar.h"

namespace pizda {
	using namespace YOBA;
	
	class RotaryControl : public ToolbarSection {
		public:
			RotaryControl();
			
			uint8_t getVariantIndex() const;
			bool isVariantSelectMode() const;
		
		protected:
			void onEventBeforeChildren(Event* event) override;
			void onFocusChanged() override;
			void onTick() override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			
			virtual std::wstring_view variantIndexToTitle(uint8_t index) = 0;
			virtual bool isVariantEditable(uint8_t index) = 0;
			virtual void onVariantChanged();
			virtual void onRotate(bool clockwise, bool big);
			virtual void onPress();
			
			void updateVariantsVisibility() const;
			void setVariantIndex(uint8_t value);
			void setVariants(std::initializer_list<Element*> elements);
			
		private:
			Layout variantsLayout {};
			uint8_t variantIndex = 0xFF;
			bool variantSelectMode = false;
			
			int64_t longPressTime = 0;
			Point pressPos { -1, -1 };
			bool wasFocusedOnLastDown = false;
			bool callOnPress = true;
			
			void onAnyDown();
			void onAnyUp();
			void onAnyRotate(bool clockwise, bool big);
	};
	
	class RotaryControlRenderableVariant : public Control {
		public:
			RotaryControlRenderableVariant() {
				setWidth(24);
			}
	};

	class RotaryControlSevenVariant : public SevenSegment {
		public:
			RotaryControlSevenVariant() {
				setAlignment(Alignment::center);

				setSegmentThickness(1);
				setSegmentLength(4);

				setInactiveColor(&Theme::bg5);
			}
	};
}