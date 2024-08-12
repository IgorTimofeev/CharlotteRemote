#pragma once

#include "yoba/elements/element.h"
#include "yoba/screen.h"
#include "../../rc_application.h"
#include "../../theme.h"

using namespace yoba;
using namespace ui;

class PFD : public Element {
	public:
		PFD() = default;

		void render(Screen &screen) override {
			auto& app = RCApplication::getInstance();
			auto& renderSize = getDesiredSize();

			int16_t centerY = renderSize.getHeight() / 2;

			const uint8_t altitudeStepUnits = 1;
			const uint8_t altitudeStepUnitsBig = 10;

			const uint8_t altitudeStepPixels = 10;

			// left
			const uint16_t leftWidth = 80;

			// Right
			const uint16_t rightWidth = 60;
			auto rightX = renderSize.getWidth() - rightWidth;

			screen.renderRectangle(
				Bounds(
					rightX,
					0,
					rightWidth,
					renderSize.getHeight()
				),
				&Theme::bg2
			);

			float altitude = app.getAltitude();

			float altitudeSnapped = altitude / (float) altitudeStepUnits;
			float altitudeSnappedInteger = floor(altitudeSnapped);
			float altitudeSnappedFractional = altitudeSnapped - altitudeSnappedInteger;

			int32_t altitudeY = centerY - (uint16_t) ((1.0f - altitudeSnappedFractional) * altitudeStepPixels);
			int32_t altitudeYFullLines = ceil((float) altitudeY / (float) altitudeStepPixels);
			altitudeY = altitudeY - altitudeYFullLines * altitudeStepPixels;

			int32_t altitudeLineValue = (int32_t) altitudeSnappedInteger * altitudeStepUnits + altitudeYFullLines * altitudeStepUnits;

			String text;
			Size textSize;
			bool isBig;

			while (altitudeY < renderSize.getHeight()) {
				isBig = altitudeLineValue % altitudeStepUnitsBig == 0;

				if (isBig) {
					auto lineWidth = 20;

					// Line
					screen.renderHorizontalLine(
						Point(rightX, altitudeY),
						lineWidth,
						&Theme::fg1
					);

					//Text
					text = String(altitudeLineValue);
					textSize = screen.measureText(text);

					screen.renderText(
						Point(rightX + lineWidth + 5, altitudeY - textSize.getHeight() / 2),
						&Theme::fg1,
						text
					);
				}
				else {
					// Line
					screen.renderHorizontalLine(
						Point(rightX, altitudeY),
						10,
						&Theme::fg1
					);
				}

				altitudeLineValue -= altitudeStepUnits;
				altitudeY += altitudeStepPixels;
			}

			// Current altitude
			text = String(altitude);
			textSize = screen.measureText(text);

			const uint8_t altitudePadding = 10;

			altitudeY = centerY - textSize.getHeight() / 2 - altitudePadding;

			screen.renderRectangle(
				Bounds(
					rightX,
					altitudeY,
					rightWidth,
					textSize.getHeight() + altitudePadding * 2
				),
				&Theme::bg3
			);

			screen.renderText(
				Point(rightX + altitudePadding, altitudeY + altitudePadding),
				&Theme::fg1,
				text
			);
		}
};