#include "pfd_left.h"
#include "../../rc_application.h"

PFDLeft::PFDLeft() {
	setClipToBounds(true);
}

void PFDLeft::renderBar(Screen &screen, float &speed, uint16_t &centerY, int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color *color) const {
	int32_t fromY = centerY + (int32_t) ceil(speed / (float) stepUnits * (float) unitPixels - ((float) fromSpeed / (float) stepUnits) * (float)unitPixels);
	int32_t toY = fromY - (int32_t) (((float) toSpeed / (float) stepUnits) * (float) unitPixels);

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

void PFDLeft::onRender(Screen &screen) {
	auto& bounds = getBounds();
	auto& app = RCApplication::getInstance();

	auto centerY = (uint16_t) (bounds.getHeight() / 2);

	screen.renderRectangle(
		bounds,
		&Theme::bg2
	);

	float speed = app.getRemoteData().getSpeed();

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

	int32_t lineValue = (int32_t) (snappedInteger + 1) * stepUnits + altitudeYFullLines * stepUnits;

	String text;
	Size textSize;
	bool isBig;
	const Color* lineColor = &Theme::fg4;

	do {
		isBig = lineValue % stepUnitsBig == 0;

		if (isBig) {
			// Line
			screen.renderHorizontalLine(
				Point(bounds.getWidth() - lineSizeBig, y),
				lineSizeBig,
				lineColor
			);

			//Text
			text = String(lineValue);
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

		lineValue -= stepUnits;
		y += unitPixels;
	}
	while (y < bounds.getHeight() && lineValue >= 0);

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
