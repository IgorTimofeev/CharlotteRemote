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

		void renderBar(Screen& screen, float& speed, uint16_t& centerY, int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color* color) {
			int32_t fromY = centerY + (int32_t) ceil(speed * (float) unitPixels) - fromSpeed * unitPixels;
			int32_t toY = fromY - toSpeed * unitPixels;

			screen.renderRectangle(
				Bounds(
					x,
					toY,
					width,
					fromY - toY
				),
				color
			);
		}

		void onRender(Screen& screen) override {
			auto& bounds = getBounds();
			auto& app = RCApplication::getInstance();

			const uint8_t stepUnits = 1;
			const uint8_t stepUnitsBig = 5;

			auto centerY = (uint16_t) (bounds.getHeight() / 2);

			screen.renderRectangle(
				bounds,
				&Theme::bg2
			);

			float speed = app.getSpeed();

			// Bars
			renderBar(
				screen,
				speed,
				centerY,
				bounds.getX2() - barWidth,
				barWidth,
				stallSpeedMin,
				stallSpeedMax,
				&Theme::red
			);

			renderBar(
				screen,
				speed,
				centerY,
				bounds.getX2() - barWidth,
				barWidth,
				landingSpeedMin,
				landingSpeedMax,
				&Theme::fg1
			);

			renderBar(
				screen,
				speed,
				centerY,
				bounds.getX2() - barWidth,
				barWidth / 2,
				takeOffSpeedMin,
				takeOffSpeedMax,
				&Theme::fg1
			);

			renderBar(
				screen,
				speed,
				centerY,
				bounds.getX2() - barWidth / 2,
				barWidth / 2,
				takeOffSpeedMin,
				takeOffSpeedMax,
				&Theme::green
			);

			renderBar(
				screen,
				speed,
				centerY,
				bounds.getX2() - barWidth,
				barWidth,
				cruiseSpeedMin,
				cruiseSpeedMax,
				&Theme::green
			);

			renderBar(
				screen,
				speed,
				centerY,
				bounds.getX2() - barWidth,
				barWidth,
				overSpeedMin,
				overSpeedMax,
				&Theme::yellow
			);

			// Lines
			float snapped = speed / (float) stepUnits;
			float snappedInteger = floor(snapped);
			float snappedFractional = snapped - snappedInteger;

			int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) unitPixels);
			int32_t altitudeYFullLines = ceil((float) y / (float) unitPixels);
			y = y - altitudeYFullLines * unitPixels;

			int32_t altitudeLineValue = (int32_t) (snappedInteger + 1) * stepUnits + altitudeYFullLines * stepUnits;

			String text;
			Size textSize;
			bool isBig;
			const Color* lineColor = &Theme::fg4;

			do {
				isBig = altitudeLineValue % stepUnitsBig == 0;

				if (isBig) {
					// Line
					screen.renderHorizontalLine(
						Point(bounds.getWidth() - lineSizeBig, y),
						lineSizeBig,
						lineColor
					);

					//Text
					text = String(altitudeLineValue);
					textSize = screen.measureText(text);

					screen.renderText(
						Point(bounds.getWidth() - lineSizeBig - 5 - textSize.getWidth(), y - textSize.getHeight() / 2),
						lineColor,
						text
					);
				}
				else {
					// Line
					screen.renderHorizontalLine(
						Point(bounds.getWidth() - lineSizeSmall, y),
						lineSizeSmall,
						lineColor
					);
				}

				altitudeLineValue -= stepUnits;
				y += unitPixels;
			}
			while (y < bounds.getHeight() && altitudeLineValue >= 0);

			// Current altitude
			char chars[5];
			sprintf(chars, "%.1f", speed);
			text = String(chars);
			textSize = screen.measureText(text);

			uint16_t currentHeight = 20;
			const uint8_t triangleWidth = 8;

			y = centerY - currentHeight / 2;

			// Triangle
			screen.renderTriangle(
				Point(
					bounds.getX2(),
					centerY
				),
				Point(
					bounds.getX2() - triangleWidth,
					y
				),
				Point(
					bounds.getX2() - triangleWidth,
					y + currentHeight - 1
				),
				&Theme::bg3
			);

			// Rect
			screen.renderRectangle(
				Bounds(
					bounds.getX(),
					y,
					bounds.getWidth() - triangleWidth,
					currentHeight
				),
				&Theme::bg3
			);

			// Text
			screen.renderText(
				Point(
					bounds.getX2() - textSize.getWidth() - 5,
					y + currentHeight / 2 - textSize.getHeight() / 2
				),
				&Theme::fg1,
				text
			);
		}

	private:
		const uint8_t unitPixels = 10;

		const uint16_t lineSizeBig = 8;
		const uint16_t lineSizeSmall = 5;

		const uint16_t barWidth = 4;

		const uint16_t stallSpeedMin = 0;
		const uint16_t stallSpeedMax = 5;

		const uint16_t landingSpeedMin = 5;
		const uint16_t landingSpeedMax = 8;

		const uint16_t takeOffSpeedMin = 8;
		const uint16_t takeOffSpeedMax = 12;

		const uint16_t cruiseSpeedMin = 12;
		const uint16_t cruiseSpeedMax = 30;

		const uint16_t overSpeedMin = 30;
		const uint16_t overSpeedMax = 100;
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
			auto x = bounds.getX();

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

			const Color* lineColor = &Theme::fg4;

			do {
				isBig = lineValue % altitudeStepUnitsBig == 0;

				if (isBig) {
					screen.renderHorizontalLine(
						Point(x, y),
						lineSizeBig,
						lineColor
					);

					//Text
					text = String(lineValue);
					textSize = screen.measureText(text);

					screen.renderText(
						Point(x + lineSizeBig + 5, y - textSize.getHeight() / 2),
						lineColor,
						text
					);
				}
				else {
					screen.renderHorizontalLine(
						Point(x, y),
						lineSizeSmall,
						lineColor
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

//			screen.renderRectangle(
//				Bounds(
//					bounds.getX(),
//					bounds.getY(),
//					2,
//					bounds.getHeight()
//				),
//				&Theme::fg1
//			);
//
//			screen.renderRectangle(
//				Bounds(
//					bounds.getX2() - 2,
//					bounds.getY(),
//					2,
//					bounds.getHeight()
//				),
//				&Theme::fg1
//			);

			// Ground
			const auto& groundMinPoint =
				yLeft < yRight
				? Point(bounds.getX(), yLeft)
				: Point(bounds.getX2(), yRight);

			auto groundMaxY = max(yLeft, yRight);

			// Triangle
			screen.renderTriangle(
				groundMinPoint,
				Point(
					bounds.getX(),
					groundMaxY
				),
				Point(
					bounds.getX2(),
					groundMaxY
				),
				&Theme::ground
			);

			// Rectangle
			if (groundMaxY < bounds.getY2()) {
				screen.renderRectangle(
					Bounds(
						bounds.getX(),
						groundMaxY,
						bounds.getWidth(),
						bounds.getHeight() - groundMaxY
					),
					&Theme::ground
				);
			}

			// Lines
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