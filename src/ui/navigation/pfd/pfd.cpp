#include "pfd.h"
#include "../../rc_application.h"

PFD::PFD() {
	setClipToBounds(true);
}

void PFD::renderTrendArrow(Screen &screen, int32_t x, int32_t y, uint16_t unitPixels, float value) {
	const uint8_t arrowSize = 3;

	auto length = (int32_t) ((float) unitPixels * value);
	auto yArrow = y - length - arrowSize;
	auto yMin = min(y, yArrow);
	auto yMax = max(y, yArrow);

	screen.renderVerticalLine(
		Point(x, yMin),
		yMax - yMin,
		&Theme::purple
	);

	screen.renderTriangle(
		Point(
			x - arrowSize,
			yArrow
		),
		Point(
			x,
			yArrow + (length > 0 ? -arrowSize : arrowSize)
		),
		Point(
			x + arrowSize,
			yArrow
		),
		&Theme::purple
	);
}

void PFD::speedRender(Screen &screen, const Bounds& bounds) {
	auto& app = RCApplication::getInstance();

	auto centerY = (uint16_t) (bounds.getHeight() / 2);

	screen.renderRectangle(
		bounds,
		&Theme::bg2
	);

	float speed = app.getRemoteData().getSpeed();

	// Bars
	const auto renderBar = [&](int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color* color) {
		int32_t fromY = centerY + (int32_t) ceil(speed * (float) speedUnitPixels - (float) fromSpeed * (float) speedUnitPixels);
		int32_t height = (toSpeed - fromSpeed) * speedUnitPixels;

		screen.renderRectangle(
			Bounds(
				x,
				fromY - height,
				width,
				height
			),
			color
		);
	};

	renderBar(
		bounds.getX2() - speedBarSize,
		speedBarSize,
		0,
		speedFlapsMin,
		&Theme::red
	);

	renderBar(
		bounds.getX2() - speedBarSize,
		speedBarSize,
		speedFlapsMin,
		speedTurbulentMin,
		&Theme::fg1
	);

	renderBar(
		bounds.getX2() - speedBarSize,
		speedBarSize / 2,
		speedTurbulentMin,
		speedFlapsMax,
		&Theme::fg1
	);

	renderBar(
		bounds.getX2() - speedBarSize / 2,
		speedBarSize / 2,
		speedTurbulentMin,
		speedFlapsMax,
		&Theme::green
	);

	renderBar(
		bounds.getX2() - speedBarSize,
		speedBarSize,
		speedFlapsMax,
		speedTurbulentMax,
		&Theme::green
	);

	renderBar(
		bounds.getX2() - speedBarSize,
		speedBarSize,
		speedTurbulentMax,
		speedSmoothMax,
		&Theme::yellow
	);

	renderBar(
		bounds.getX2() - speedBarSize,
		speedBarSize,
		speedSmoothMax,
		speedSmoothMax * 2,
		&Theme::red
	);

	// Lines
	float snapped = speed / (float) speedStepUnits;
	float snappedInteger = floor(snapped);
	float snappedFractional = snapped - snappedInteger;

	int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) speedUnitPixels);
	int32_t altitudeYFullLines = ceil((float) y / (float) speedUnitPixels);
	y = y - altitudeYFullLines * speedUnitPixels;

	int32_t lineValue = (int32_t) (snappedInteger + 1) * speedStepUnits + altitudeYFullLines * speedStepUnits;

	String text;
	Size textSize;
	bool isBig;
	const Color* lineColor = &Theme::fg3;

	do {
		isBig = lineValue % speedStepUnitsBig == 0;

		if (isBig) {
			// Line
			screen.renderHorizontalLine(
				Point(bounds.getWidth() - speedBarSize - lineSizeBig, y),
				lineSizeBig,
				lineColor
			);

			//Text
			text = String(lineValue);
			textSize = screen.measureText(text);

			screen.renderText(
				Point(bounds.getWidth() - speedBarSize - lineSizeBig - 5 - textSize.getWidth(), y - textSize.getHeight() / 2),
				lineColor,
				text
			);
		}
		else {
			// Line
			screen.renderHorizontalLine(
				Point(bounds.getWidth() - speedBarSize - lineSizeSmall, y),
				lineSizeSmall,
				lineColor
			);
		}

		lineValue -= speedStepUnits;
		y += speedUnitPixels;
	}
	while (y < bounds.getHeight() && lineValue >= 0);

	// Trend
	renderTrendArrow(
		screen,
		bounds.getX2() - speedBarSize - lineSizeBig,
		centerY,
		speedUnitPixels,
		app.getLocalData().getSpeedTrend()
	);

	// Current speed
	char chars[5];
	sprintf(chars, "%.0f", speed);
	text = String(chars);
	textSize = screen.measureText(text);

	y = centerY - currentValueHeight / 2;

	// Triangle
	screen.renderTriangle(
		Point(
			bounds.getX2(),
			centerY
		),
		Point(
			bounds.getX2() - currentValueTriangleSize,
			y
		),
		Point(
			bounds.getX2() - currentValueTriangleSize,
			y + currentValueHeight - 1
		),
		&Theme::bg3
	);

	// Rect
	screen.renderRectangle(
		Bounds(
			bounds.getX(),
			y,
			bounds.getWidth() - currentValueTriangleSize,
			currentValueHeight
		),
		&Theme::bg3
	);

	// Text
	screen.renderText(
		Point(
			bounds.getX2() - textSize.getWidth() - currentValueTriangleSize,
			y + currentValueHeight / 2 - textSize.getHeight() / 2
		),
		&Theme::fg1,
		text
	);
}

void PFD::horizonRender(Screen &screen, const Bounds& bounds) {
	const auto& center = Point(
		bounds.getX() + bounds.getWidth() / 2,
		bounds.getY() + bounds.getHeight() / 2
	);

	auto& app = RCApplication::getInstance();
	auto pitch = app.getRemoteData().getPitch();
	auto roll = app.getRemoteData().getRoll();
	auto yaw = app.getRemoteData().getYaw();

	const auto horizontalScale = 2.5f;
	const auto verticalScale = 1.5f;

	const auto horizontalPixels = (int32_t) ((float) bounds.getWidth() * horizontalScale);
	const auto verticalPixels = (int32_t) ((float) bounds.getHeight() * verticalScale);

	const auto& radiusRollRotated = Point(horizontalPixels, 0).rotate(roll);
	const auto& radiusPitchRotated = Point(verticalPixels, 0).rotate(pitch);

	screen.renderRectangle(
		bounds,
		&Theme::sky
	);

	// Ground
	auto left = Point(
		center.getX() - radiusRollRotated.getX(),
		center.getY() - radiusPitchRotated.getY() - radiusRollRotated.getY()
	);

	auto right = Point(
		center.getX() + radiusRollRotated.getX(),
		center.getY() - radiusPitchRotated.getY() + radiusRollRotated.getY()
	);

	auto groundMaxY = max(left.getY(), right.getY());

	// Triangle
	screen.renderTriangle(
		left.getY() < right.getY()
		? left
		: right,
		Point(
			left.getX(),
			groundMaxY
		),
		Point(
			right.getX(),
			groundMaxY
		),
		&Theme::ground
	);

	// Rectangle

	// Bottom
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
	// Left
	else if (left.getY() < bounds.getY() && left.getX() > bounds.getX()) {
		screen.renderRectangle(
			Bounds(
				bounds.getX(),
				bounds.getY(),
				left.getX() - bounds.getX(),
				bounds.getHeight()
			),
			&Theme::ground
		);
	}
		// Right
	else if (right.getY() < bounds.getY() && right.getX() < bounds.getX2()) {
		screen.renderRectangle(
			Bounds(
				right.getX(),
				bounds.getY(),
				bounds.getX2() - right.getX(),
				bounds.getHeight()
			),
			&Theme::ground
		);
	}

	// Lines
	float lineSmall = 14;
	float lineMiddle = 50;
	float lineBig = 20;
	uint8_t lineAngleStepDeg = 5;
	float lineAngleStepRad = radians(lineAngleStepDeg);
	float linesInTotal = floor(((float) HALF_PI) / lineAngleStepRad);
	float linePixelStep = (float) verticalPixels / linesInTotal;

	auto pizdaX = (float) (right.getX() - left.getX());
	auto pizdaY = (float) (right.getY() - left.getY());

	auto pizdaDistance = sqrt(pizdaX * pizdaX + pizdaY * pizdaY);

	auto pizdaXNorm = pizdaX / pizdaDistance;
	auto pizdaYNorm = pizdaY / pizdaDistance;

	auto pizdaPerpX = -pizdaYNorm;
	auto pizdaPerpY = pizdaXNorm;

	auto pizdaCenterX = (float) left.getX() + pizdaX / 2;
	auto pizdaCenterY = (float) left.getY() + pizdaY / 2;

	String text;
	Size textSize;

	for (int32_t lineAngleDeg = -90; lineAngleDeg <= 90; lineAngleDeg += lineAngleStepDeg) {
		float lineSize = lineAngleDeg == 0 ? lineMiddle : (lineAngleDeg % 10 == 0 ? lineBig : lineSmall);
		float lineY = (float) lineAngleDeg / (float) lineAngleStepDeg * linePixelStep;

		auto govnoX = pizdaCenterX + pizdaPerpX * lineY;
		auto govnoY = pizdaCenterY + pizdaPerpY * lineY;

		auto lineLeft = Point(
			(int32_t) (govnoX + pizdaXNorm * -lineSize),
			(int32_t) (govnoY + pizdaYNorm * -lineSize)
		);

		auto lineRight = Point(
			(int32_t) (govnoX + pizdaXNorm * lineSize),
			(int32_t) (govnoY + pizdaYNorm * lineSize)
		);

		screen.renderLine(
			lineLeft,
			lineRight,
			&Theme::fg1
		);

		if (lineAngleDeg != 0 && lineAngleDeg % 10 == 0) {
			text = String(-lineAngleDeg);
			textSize = screen.measureText(text);

			screen.renderText(
				Point(
					lineLeft.getX() - textSize.getWidth() - 8,
					lineLeft.getY() - textSize.getHeight() / 2
				),
				&Theme::fg1,
				text
			);

			screen.renderText(
				Point(
					lineRight.getX() + 8,
					lineRight.getY() - textSize.getHeight() / 2
				),
				&Theme::fg1,
				text
			);
		}
	}

	// Bird
	const uint8_t birdWidth = 38;
	const uint8_t birdThickness = 2;
	const uint8_t birdHeight = 8;
	const uint8_t birdCenterOffset = 20;

	// Left
	screen.renderRectangle(
		Bounds(
			center.getX() - birdCenterOffset - birdThickness,
			center.getY() - birdThickness / 2,
			birdThickness,
			birdHeight
		),
		&Theme::bg1
	);

	screen.renderRectangle(
		Bounds(
			center.getX() - birdCenterOffset - birdThickness - birdWidth,
			center.getY() - birdThickness / 2,
			birdWidth,
			birdThickness
		),
		&Theme::bg1
	);

	// Right
	screen.renderRectangle(
		Bounds(
			center.getX() + birdCenterOffset,
			center.getY() - birdThickness / 2,
			birdThickness,
			birdHeight
		),
		&Theme::bg1
	);

	screen.renderRectangle(
		Bounds(
			center.getX() + birdCenterOffset + birdThickness,
			center.getY() - birdThickness / 2,
			birdWidth,
			birdThickness
		),
		&Theme::bg1
	);

	// Dot
	screen.renderRectangle(
		Bounds(
			center.getX() + birdThickness / 2 - birdThickness / 2,
			center.getY() - birdThickness / 2,
			birdThickness,
			birdThickness
		),
		&Theme::bg1
	);

	// Radio
	char buffer[255];

	sprintf(buffer, "RSSI: %.2f dBm", app.getTransceiver().getRssi());
	screen.renderText(
		Point(bounds.getX() + 10, bounds.getY() + 10),
		&Theme::fg1,
		String(buffer)
	);

	sprintf(buffer, "SNR: %.2f dB", app.getTransceiver().getSnr());
	screen.renderText(
		Point(bounds.getX() + 10, bounds.getY() + 20),
		&Theme::fg1,
		String(buffer)
	);
}

void PFD::altitudeRender(Screen &screen, const Bounds& bounds) {
	auto& app = RCApplication::getInstance();

	uint16_t altitudeHeight = bounds.getHeight() - pressureHeight;

	auto centerY = (int16_t) (altitudeHeight / 2);
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

	float altitude = app.getRemoteData().getAltitude();
	float snapped = altitude / (float) altitudeStepUnits;
	float snappedInteger = floor(snapped);
	float snappedFractional = snapped - snappedInteger;

	int32_t y = centerY - (uint16_t) ((1.0f - snappedFractional) * (float) altitudeUnitPixels);
	auto yFullLines = (int32_t) ceil((float) y / (float) altitudeUnitPixels);
	y = y - yFullLines * altitudeUnitPixels;

	int32_t lineValue = (int32_t) (snappedInteger + 1) * altitudeStepUnits + yFullLines * altitudeStepUnits;

	String text;
	Size textSize;
	bool isBig;

	const Color* lineColor = &Theme::fg3;

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
		y += altitudeUnitPixels;
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

	// Trend
	renderTrendArrow(
		screen,
		bounds.getX() + lineSizeBig,
		centerY,
		altitudeUnitPixels,
		app.getLocalData().getAltitudeTrend()
	);

	// Current altitude
	char buffer[8];
	sprintf(buffer, "%.0f", altitude);
	text = String(buffer);
	textSize = screen.measureText(text);

	y = centerY - currentValueHeight / 2;

	// Triangle
	screen.renderTriangle(
		Point(
			x,
			centerY
		),
		Point(
			x + currentValueTriangleSize - 1,
			y
		),
		Point(
			x + currentValueTriangleSize - 1,
			y + currentValueHeight - 1
		),
		&Theme::bg3
	);

	// Rect
	screen.renderRectangle(
		Bounds(
			x + currentValueTriangleSize,
			y,
			bounds.getWidth() - currentValueTriangleSize,
			currentValueHeight
		),
		&Theme::bg3
	);

	// Text
	screen.renderText(
		Point(
			x + currentValueTriangleSize,
			y + currentValueHeight / 2 - textSize.getHeight() / 2
		),
		&Theme::fg1,
		text
	);

	// Pressure
	auto pressureBg = &Theme::bg3;
	auto pressureFg = &Theme::ocean;

	y = bounds.getHeight() - pressureHeight;

	switch (app.getLocalData().getAltimeterMode()) {
		case QNH:
			snprintf(buffer, 5, "%d", (uint16_t) app.getLocalData().getAltimeterPressure());

			break;

		case QNE:
			snprintf(buffer, 4, "STD");
			pressureBg = &Theme::yellow;
			pressureFg = &Theme::bg1;

			break;
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

void PFD::verticalSpeedRender(Screen &screen, const Bounds &bounds) {
	auto& app = RCApplication::getInstance();
	auto centerY = bounds.getY() + bounds.getHeight() / 2;

	// Background
	screen.renderRectangle(
		bounds,
		&Theme::bg3
	);

	// Lines
	const Color* lineColor = &Theme::fg4;

	int32_t y;
	int32_t lineValue = 0;

	Size textSize;
	String text;
	bool isBig;

	auto renderLine = [&]() {
		isBig = lineValue % verticalSpeedStepUnitsBig == 0;

		if (isBig) {
			screen.renderHorizontalLine(
				Point(
					bounds.getX(),
					y
				),
				lineSizeBig,
				lineColor
			);

			text = String(lineValue / 1000);
			textSize = screen.measureText(text);

			screen.renderText(
				Point(
					bounds.getX() + lineSizeBig + 5,
					y - textSize.getHeight() / 2
				),
				lineColor,
				text
			);
		}
		else {
			screen.renderHorizontalLine(
				Point(
					bounds.getX(),
					y
				),
				lineSizeSmall,
				lineColor
			);
		}

		lineValue += verticalSpeedStepUnits;
	};

	for (y = centerY; y >= bounds.getY(); y -= verticalSpeedUnitPixels)
		renderLine();

	lineValue = verticalSpeedStepUnits;

	for (y = centerY + verticalSpeedUnitPixels; y < bounds.getY2(); y += verticalSpeedUnitPixels)
		renderLine();

	// Current value
	screen.renderLine(
		Point(bounds.getX(), centerY - (int32_t) (app.getLocalData().getVerticalSpeed() / 100.0f * (float) verticalSpeedUnitPixels)),
		Point(bounds.getX2(), centerY - (int32_t) (app.getLocalData().getVerticalSpeed() / 100.0f * (float) verticalSpeedRightUnitPixels)),
		&Theme::green
	);
}

void PFD::onRender(Screen &screen) {
	auto& bounds = getBounds();

	horizonRender(screen, Bounds(
		bounds.getX() + speedWidth,
		bounds.getY(),
		bounds.getWidth() - speedWidth - altitudeWidth - verticalSpeedWidth,
		bounds.getHeight()
	));

	speedRender(screen, Bounds(
		bounds.getX(),
		bounds.getY(),
		speedWidth,
		bounds.getHeight()
	));

	altitudeRender(screen, Bounds(
		bounds.getX2() - altitudeWidth - verticalSpeedWidth,
		bounds.getY(),
		altitudeWidth,
		bounds.getHeight()
	));

	verticalSpeedRender(screen, Bounds(
		bounds.getX2() - verticalSpeedWidth,
		bounds.getY(),
		verticalSpeedWidth,
		bounds.getHeight()
	));
}
