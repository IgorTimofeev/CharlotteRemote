#pragma once

#include "yoba/elements/element.h"
#include "yoba/screen.h"
#include "../../rc_application.h"
#include "../../theme.h"

using namespace yoba;
using namespace ui;

class PFDLeft : public Element {
	public:
		PFDLeft() {
			setClipToBounds(true);
		}

		void onRender(Screen &screen) override {
			const uint8_t stepUnits = 1;
			const uint8_t stepUnitsBig = 10;
			const uint8_t stepPixels = 10;

			auto& bounds = getBounds();
			auto& app = RCApplication::getInstance();

			auto centerY = (int16_t) (bounds.getHeight() / 2);
			auto x = 0;

			screen.renderRectangle(
				Bounds(
					x,
					0,
					bounds.getWidth(),
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
						Point(bounds.getWidth() - lineSizeBig, y),
						lineSizeBig,
						&Theme::fg1
					);

					//Text
					text = String(altitudeLineValue);
					textSize = screen.measureText(text);

					screen.renderText(
						Point(bounds.getWidth() - lineSizeBig - 5 - textSize.getWidth(), y - textSize.getHeight() / 2),
						&Theme::fg1,
						text
					);
				}
				else {
					// Line
					screen.renderHorizontalLine(
						Point(bounds.getWidth() - lineSizeSmall, y),
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
					bounds.getWidth(),
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

	private:
		const uint16_t lineSizeBig = 8;
		const uint16_t lineSizeSmall = 5;
};


class PFDRight : public Element {
	public:
		PFDRight() {
			setClipToBounds(true);
		}

		void onRender(Screen &screen) override {
			const uint8_t altitudeStepUnits = 1;
			const uint8_t altitudeStepUnitsBig = 5;
			const uint8_t altitudeStepPixels = 10;
			const uint8_t pressureHeight = 16;

			auto& app = RCApplication::getInstance();
			auto& bounds = getBounds();

			uint16_t altitudeHeight = bounds.getHeight() - pressureHeight;

			auto centerY = (int16_t) (altitudeHeight / 2);

			// Right
			auto x = bounds.getWidth() - bounds.getWidth();

			screen.renderRectangle(
				Bounds(
					x,
					0,
					bounds.getWidth(),
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

			do {
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
			while (y < altitudeHeight && lineValue >= 0);

			// Ground
			if (y < altitudeHeight && lineValue < 0) {
				const int8_t groundSpacing = 5;
				auto groundPoint1 = Point(x, y + groundSpacing);
				auto groundPoint2 = Point(x + groundSpacing, y);

				do {
					screen.renderLine(
						groundPoint1,
						groundPoint2,
						&Theme::yellow
					);

					// 1
					if (groundPoint1.getY() < bounds.getY2()) {
						groundPoint1.setY(groundPoint1.getY() + groundSpacing);
					}
					else {
						groundPoint1.setX(groundPoint1.getX() + groundSpacing);
					}

					// 2
					if (groundPoint2.getX() < bounds.getX2()) {
						groundPoint2.setX(groundPoint2.getX() + groundSpacing);
					}
					else {
						groundPoint2.setY(groundPoint2.getY() + groundSpacing);
					}
				}
				while (groundPoint1.getX() < bounds.getX2() - groundSpacing);
			}

			// Current altitude
			char buffer[8];
			sprintf(buffer, "%.1f", altitude);
			text = String(buffer);
			textSize = screen.measureText(text);

			altitudeHeight = 20;
			y = centerY - altitudeHeight / 2;

			const uint8_t triangleWidth = 8;

			// Triangle
			screen.renderTriangle(
				Point(
					x,
					centerY
				),
				Point(
					x + triangleWidth - 1,
					y
				),
				Point(
					x + triangleWidth - 1,
					y + altitudeHeight - 1
				),
				&Theme::bg3
			);

			// Rect
			screen.renderRectangle(
				Bounds(
					x + triangleWidth,
					y,
					bounds.getWidth() - triangleWidth,
					altitudeHeight
				),
				&Theme::bg3
			);

			// Text
			screen.renderText(
				Point(
					x + triangleWidth,
					y + altitudeHeight / 2 - textSize.getHeight() / 2
				),
				&Theme::fg1,
				text
			);

			// Pressure
			auto pressureBg = &Theme::bg3;
			auto pressureFg = &Theme::ocean;

			y = bounds.getHeight() - pressureHeight;

			if (app.isPressureSTD()) {
				snprintf(buffer, 4, "STD");

				pressureBg = &Theme::yellow;
				pressureFg = &Theme::bg1;
			}
			if (app.isPressureHPA()) {
				snprintf(buffer, 5, "%d", (uint16_t) app.getPressure());
			}
			else {
				snprintf(buffer, 7, "%.2fin", app.getPressure());
			}

			// Rect
			screen.renderRectangle(
				Bounds(
					x,
					y,
					bounds.getWidth(),
					pressureHeight
				),
				pressureBg
			);

			// Text
			text = String(buffer);
			textSize = screen.measureText(text);

			screen.renderText(
				Point(
					x + bounds.getWidth() / 2 - textSize.getWidth() / 2,
					y + pressureHeight / 2 - textSize.getHeight() / 2
				),
				pressureFg,
				text
			);
		}

	private:
		const uint16_t lineSizeBig = 8;
		const uint16_t lineSizeSmall = 5;
};

class PFDHorizon : public Element {
	public:
		PFDHorizon() {
			setClipToBounds(true);
		}

		void onRender(Screen &screen) override {
			auto& bounds = getBounds();
			int32_t centerY = bounds.getHeight() / 2;

			auto& app = RCApplication::getInstance();
			auto pitch = app.getPitch();
			auto roll = app.getRoll();
			auto yaw = app.getYaw();

			auto pitchPixels = (int32_t) (pitch * (float) centerY);
			auto rollPixels = (int32_t) (roll * (float) centerY);

			int32_t yLeft = centerY - pitchPixels - rollPixels;
			int32_t yRight = centerY - pitchPixels + rollPixels;

			screen.renderRectangle(
				bounds,
				&Theme::sky
			);

			screen.renderLine(
				Point(
					bounds.getX(),
					yLeft
				),
				Point(
					bounds.getX2(),
					yRight
				),
				&Theme::fg1
			);
		}
};