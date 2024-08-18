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

		const uint16_t sideWidth = 40;
		const uint16_t lineSizeBig = 8;
		const uint16_t lineSizeSmall = 5;

		void renderSpeed(Screen &screen) {
			const uint8_t stepUnits = 1;
			const uint8_t stepUnitsBig = 10;
			const uint8_t stepPixels = 10;

			auto& app = RCApplication::getInstance();
			auto& bounds = getBounds();

			auto centerY = (int16_t) (bounds.getHeight() / 2);
			auto x = 0;

			screen.renderRectangle(
				Bounds(
					x,
					0,
					sideWidth,
					bounds.getHeight()
				),
				&Theme::bg2
			);

			float speed = app.getSpeed();

			float snapped = speed / (float) stepUnits;
			float snappedInteger = floor(snapped);
			float snappedFractional = snapped - snappedInteger;

			int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) stepPixels);
			int32_t altitudeYFullLines = ceil((float) y / (float) stepPixels);
			y = y - altitudeYFullLines * stepPixels;

			int32_t altitudeLineValue = (int32_t) snappedInteger * stepUnits + altitudeYFullLines * stepUnits;

			String text;
			Size textSize;
			bool isBig;

			while (y < bounds.getHeight()) {
				isBig = altitudeLineValue % stepUnitsBig == 0;

				if (isBig) {
					// Line
					screen.renderHorizontalLine(
						Point(sideWidth - lineSizeBig, y),
						lineSizeBig,
						&Theme::fg1
					);

					//Text
					text = String(altitudeLineValue);
					textSize = screen.measureText(text);

					screen.renderText(
						Point(sideWidth - lineSizeBig - 5 - textSize.getWidth(), y - textSize.getHeight() / 2),
						&Theme::fg1,
						text
					);
				}
				else {
					// Line
					screen.renderHorizontalLine(
						Point(sideWidth - lineSizeSmall, y),
						lineSizeSmall,
						&Theme::fg1
					);
				}

				altitudeLineValue -= stepUnits;
				y += stepPixels;
			}

			// Current altitude
			char number[5];
			sprintf(number, "%.1f", speed);

			text = String(number);
			textSize = screen.measureText(text);

			const Size altitudePadding = Size(5, 5);

			y = centerY - textSize.getHeight() / 2 - altitudePadding.getHeight();

			screen.renderRectangle(
				Bounds(
					x,
					y,
					sideWidth,
					textSize.getHeight() + altitudePadding.getHeight() * 2
				),
				&Theme::bg3
			);

			screen.renderText(
				Point(x + altitudePadding.getWidth(), y + altitudePadding.getHeight()),
				&Theme::fg1,
				text
			);
		}

		void renderAltitude(Screen &screen) {
			const uint8_t altitudeStepUnits = 1;
			const uint8_t altitudeStepUnitsBig = 5;
			const uint8_t altitudeStepPixels = 10;
			const uint8_t baroHeight = 16;

			auto& app = RCApplication::getInstance();
			auto& bounds = getBounds();

			uint16_t altitudeHeight = bounds.getHeight() - baroHeight;

			auto centerY = (int16_t) (altitudeHeight / 2);

			// Right
			auto x = bounds.getWidth() - sideWidth;

			screen.renderRectangle(
				Bounds(
					x,
					0,
					sideWidth,
					altitudeHeight
				),
				&Theme::bg2
			);

			float altitude = app.getAltitude();
			float snapped = altitude / (float) altitudeStepUnits;
			float snappedInteger = floor(snapped);
			float snappedFractional = snapped - snappedInteger;

			int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) altitudeStepPixels);
			auto yFullLines = (int32_t) ceil((float) y / (float) altitudeStepPixels);
			y = y - yFullLines * altitudeStepPixels;

			int32_t lineValue = (int32_t) (snappedInteger + 1) * altitudeStepUnits + yFullLines * altitudeStepUnits;

			String text;
			Size textSize;
			bool isBig;

			const Color* color = &Theme::fg4;

			while (y < altitudeHeight) {
				isBig = lineValue % altitudeStepUnitsBig == 0;

				if (isBig) {
					screen.renderHorizontalLine(
						Point(x, y),
						lineSizeBig,
						color
					);

					//Text
					text = String(lineValue);
					textSize = screen.measureText(text);

					screen.renderText(
						Point(x + lineSizeBig + 5, y - textSize.getHeight() / 2),
						color,
						text
					);
				}
				else {
					screen.renderHorizontalLine(
						Point(x, y),
						lineSizeSmall,
						color
					);
				}

				lineValue -= altitudeStepUnits;
				y += altitudeStepPixels;
			}

			// Current altitude
			char buffer[8];
			sprintf(buffer, "%.1f", altitude);
			text = String(buffer);
			textSize = screen.measureText(text);

			altitudeHeight = 20;
			y = centerY - altitudeHeight / 2;

			// Rect
			screen.renderRectangle(
				Bounds(
					x,
					y,
					sideWidth,
					altitudeHeight
				),
				&Theme::bg3
			);

			// Text
			screen.renderText(
				Point(
					x,
					y + altitudeHeight / 2 - textSize.getHeight() / 2
				),
				&Theme::fg1,
				text
			);

			// Baro
			auto baro = app.getBaro();

			y = bounds.getHeight() - baroHeight;

			screen.renderRectangle(
				Bounds(
					x,
					y,
					sideWidth,
					baroHeight
				),
				&Theme::ocean
			);

			snprintf(buffer, 7, "%.2f", baro);
			text = String(buffer);
			textSize = screen.measureText(text);

			screen.renderText(
				Point(x, y + baroHeight / 2 -  textSize.getHeight() / 2),
				&Theme::bg1,
				text
			);
		}

		void render(Screen &screen) override {
			renderSpeed(screen);
			renderAltitude(screen);
		}
};