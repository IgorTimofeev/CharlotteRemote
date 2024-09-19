#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"
#include "../../../../lib/YOBA/src/ui/sevenSegment.h"
#include "../../../../lib/YOBA/src/ui/rotaryKnob.h"

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/rows.h"
#include "../../rc_application.h"

using namespace yoba;

namespace pizdanc {
	class APModule : public Layout {
		public:
			explicit APModule(const wchar_t* name) {
				background.setBackground(&Theme::bg2);
				background.setCornerRadius(5);
				addChild(&background);

				rows.setMargin(Margin(10));
				rows.setSpacing(20);

				// Seven segment
				setIndicatorStyle(sevenSegment);
				title.addChild(&sevenSegment);

				// Title
				title.setAlignment(Alignment::Center);
				title.setText(name);
				rows.addChild(&title);

				// Rotary knob
				setRotaryStyle(rotaryKnob);
				rotaryKnob.setAlignment(Alignment::Center);
				rows.addChild(&rotaryKnob);

				addChild(&rows);
			}

			Rectangle background = Rectangle();
			StackLayout rows = StackLayout();
			RotaryKnob rotaryKnob = RotaryKnob();
			SevenSegment sevenSegment = SevenSegment();
			Titler title = Titler();

		private:
			static void setIndicatorStyle(SevenSegment& indicator) {
				indicator.setBackground(&Theme::bg4);
				indicator.setForeground(&Theme::fg1);
				indicator.setDigitCount(4);
				indicator.setSegmentThickness(2);
				indicator.setSegmentLength(10);
			}

			static void setRotaryStyle(RotaryKnob& rotaryKnob) {
				rotaryKnob.setBackground(&Theme::bg3);
				rotaryKnob.setCenterColor(&Theme::bg5);
				rotaryKnob.setLineColor(&Theme::yellow);
				rotaryKnob.setSize(Size(50, 50));
			}
	};

	class AutopilotPage : public Page {
		public:
			AutopilotPage() {
				columns.setAlignment(Alignment::Center);
				columns.setOrientation(Orientation::Horizontal);
				columns.setSpacing(20);

				spd.rotaryKnob.addOnRotate([&](float delta) {
					spd.sevenSegment.setValue(spd.sevenSegment.getValue() + (delta > 0 ? 1 : -1));
					Serial.printf("Delta: %f, new angle: %f\n", delta, degrees(spd.rotaryKnob.getAngle()));
				});

				columns.addChild(&spd);

				columns.addChild(&alt);

				columns.addChild(&baro);

				addChild(&columns);
			}

			StackLayout columns = StackLayout();

		private:


			APModule spd = APModule(L"Speed");
			APModule alt = APModule(L"Alt");
			APModule baro = APModule(L"Baro");
	};
}