#include "pfd_horizon.h"
#include "../../rc_application.h"

PFDHorizon::PFDHorizon() {
	setClipToBounds(true);
}

void PFDHorizon::onRender(Screen &screen) {
	auto& bounds = getBounds();

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
}
