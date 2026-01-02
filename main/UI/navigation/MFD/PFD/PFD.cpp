#include "PFD.h"
#include <numbers>

#include "rc.h"
#include <units.h>
#include <UI/elements/spatial/runwayMesh.h>

namespace pizda {
	PFDScene::PFDScene() {
		setClipToBounds(true);
		
		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();
		const auto& nd = rc.getNavigationData();

		setFOV(toRadians(settings.personalization.MFD.PFD.FOV));
		
		// Runways
		for (const auto& airport : nd.airports) {
			for (const auto& runway : airport.runways) {
				addElement(new RunwayMesh(
					&runway,
					&Theme::bg1
				));
			}
		}
	}

	void PFDScene::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		const auto& settings = rc.getSettings();
		const auto& ad = rc.getAircraftData();

		const auto& center = bounds.getCenter();

		const auto projectionPlaneDistance = getProjectionPlaneDistance();

		const auto pitchPixelOffsetProjected = std::tanf(ad.computed.pitchRad) * projectionPlaneDistance;
		const auto& pitchPixelOffsetRotated = Vector2F(0, pitchPixelOffsetProjected).rotate(-ad.computed.rollRad);
		const auto diagonal = std::sqrtf(bounds.getWidth() * bounds.getWidth() + bounds.getHeight() * bounds.getHeight());
		const auto& horizonRollRotated = Vector2F(diagonal / 2.f, 0).rotate(-ad.computed.rollRad);

		const auto& horizonLeft = Point(
			center.getX() + static_cast<int32_t>(-horizonRollRotated.getX() + pitchPixelOffsetRotated.getX()),
			center.getY() + static_cast<int32_t>(-horizonRollRotated.getY() + pitchPixelOffsetRotated.getY())
		);

		const auto& horizonRight = Point(
			center.getX() + static_cast<int32_t>(horizonRollRotated.getX() + pitchPixelOffsetRotated.getX()),
			center.getY() + static_cast<int32_t>(horizonRollRotated.getY() + pitchPixelOffsetRotated.getY())
		);

		const auto& horizonVec = static_cast<Vector2F>(horizonRight - horizonLeft);
		const auto& horizonVecNorm = horizonVec.normalize();
		const auto& horizonVecPerp = horizonVecNorm.counterClockwisePerpendicular();
		const auto& horizonCenter = static_cast<Vector2F>(horizonLeft) + horizonVec / 2.0f;

		// Background
		renderSyntheticVisionBackground(
			renderer,
			bounds,
			horizonLeft,
			horizonRight
		);
		
		Scene::onRender(renderer, bounds);

		// Roll overlay
		renderTurnCoordinatorOverlay(
			renderer,
			bounds,
			ad
		);

		// Pitch overlay
		renderPitchOverlay(
			renderer,
			Bounds(
				bounds.getX(),
				bounds.getY() + PFD::pitchOverlayMarginTop,
				bounds.getWidth(),
				bounds.getHeight() - PFD::pitchOverlayMarginTop - PFD::yawOverlayHeight
			),
			ad,
			pitchPixelOffsetProjected,
			projectionPlaneDistance,
			horizonLeft,
			horizonRight,
			horizonVec,
			horizonVecNorm,
			horizonVecPerp,
			horizonCenter
		);

		// Yaw overlay
		if (!settings.personalization.MFD.ND.visible) {
			renderYawOverlay(
				renderer,
				Bounds(
					bounds.getX(),
					bounds.getY2() - PFD::yawOverlayHeight + 1,
					bounds.getWidth(),
					PFD::yawOverlayHeight
				),
				settings,
				ad
			);
		}

		// Wind
		if (ad.raw.groundSpeedKt > PFD::windVisibilityGroundSpeed) {
			const auto& windPosition = Point(
				bounds.getX() + 6,
				bounds.getY2() - 18
			);

			constexpr uint8_t textOffset = 4;
			constexpr uint8_t arrowSize = 16;

			const auto text = std::to_wstring(static_cast<uint16_t>(ad.raw.windSpeed));

			const auto arrowVec = Vector2F(0, arrowSize).rotate(ad.computed.windDirectionRad + std::numbers::pi_v<float> + ad.computed.yawRad);
			const auto arrowVecNorm = arrowVec.normalize();
			const auto arrowVecPerp = arrowVecNorm.counterClockwisePerpendicular();

			const auto arrowCenter = Vector2F (
				windPosition.getX() + arrowSize / 2,
				windPosition.getY() - Theme::fontSmall.getHeight() - textOffset - arrowSize / 2
			);

			const auto arrowToVec = arrowCenter - arrowVec / 2.f;

			renderer->renderLine(
				static_cast<Point>(arrowCenter + arrowVec / 2.f),
				static_cast<Point>(arrowToVec),
				&Theme::ground2
			);

			constexpr uint8_t triangleWidth = 2;
			constexpr uint8_t triangleHeight = 3;

			renderer->renderFilledTriangle(
				static_cast<Point>(arrowToVec),
				static_cast<Point>(arrowToVec + arrowVecNorm * triangleHeight - arrowVecPerp * triangleWidth),
				static_cast<Point>(arrowToVec + arrowVecNorm * triangleHeight + arrowVecPerp * triangleWidth),
				&Theme::ground2
			);

			renderer->renderString(
				Point(
					arrowCenter.getX() - Theme::fontSmall.getWidth(text) / 2,
					windPosition.getY() - Theme::fontSmall.getHeight()
				),
				&Theme::fontSmall,
				&Theme::ground2,
				text
			);
		}

		// Flight director
		if (settings.personalization.MFD.PFD.flightDirector) {
			const uint16_t flightDirectorLength = static_cast<uint32_t>(std::min(bounds.getWidth(), bounds.getHeight())) * PFD::flightDirectorLengthFactor / 100;
			const auto flightDirectorLengthHalfF = static_cast<float>(flightDirectorLength) / 2.f;

			// Horizontal
			auto flightDirectorRectBounds = Bounds(
				center.getX() - flightDirectorLength / 2,
				center.getY()
					- static_cast<int32_t>(std::clamp(
						std::tanf(ad.computed.autopilotPitchRad - ad.computed.pitchRad) * projectionPlaneDistance,
						-flightDirectorLengthHalfF,
						flightDirectorLengthHalfF
					))
					- PFD::flightDirectorThickness / 2,
				flightDirectorLength,
				PFD::flightDirectorThickness
			);

			renderer->renderFilledRectangle(flightDirectorRectBounds, &Theme::purple);

			// Vertical
			flightDirectorRectBounds.setX(
				center.getX()
					+ static_cast<int32_t>(std::clamp(
						(ad.computed.autopilotRollRad - ad.computed.rollRad) / toRadians(30) * flightDirectorLengthHalfF,
						-flightDirectorLengthHalfF,
						flightDirectorLengthHalfF
					))
					- PFD::flightDirectorThickness / 2
			);

			flightDirectorRectBounds.setY(center.getY() - flightDirectorLength / 2);
			flightDirectorRectBounds.setWidth(PFD::flightDirectorThickness);
			flightDirectorRectBounds.setHeight(flightDirectorLength);

			renderer->renderFilledRectangle(flightDirectorRectBounds, &Theme::purple);
		}

		// Flight path vector
		if (ad.computed.airspeedKt > PFD::speedFlapsMin) {
			const auto& FPVPosition = Point(
				static_cast<int32_t>(horizonCenter.getX() + std::tanf(ad.computed.flightPathVectorYawRad) * projectionPlaneDistance),
				static_cast<int32_t>(horizonCenter.getY() - std::tanf(ad.computed.flightPathVectorPitchRad) * projectionPlaneDistance)
			);

			// Circle
			for (uint8_t i = 0; i < PFD::flightPathVectorLineThickness; i++) {
				renderer->renderCircle(
					FPVPosition,
					PFD::flightPathVectorRadius - i,
					&Theme::bg1
				);
			}

			// Left line
			renderer->renderFilledRectangle(
				Bounds(
					FPVPosition.getX() - PFD::flightPathVectorRadius - PFD::flightPathVectorLineLength,
					FPVPosition.getY() - PFD::flightPathVectorLineThickness / 2,
					PFD::flightPathVectorLineLength,
					PFD::flightPathVectorLineThickness
				),
				&Theme::bg1
			);

			// Right line
			renderer->renderFilledRectangle(
				Bounds(
					FPVPosition.getX() + PFD::flightPathVectorRadius,
					FPVPosition.getY() - PFD::flightPathVectorLineThickness / 2,
					PFD::flightPathVectorLineLength,
					PFD::flightPathVectorLineThickness
				),
				&Theme::bg1
			);
		}

		// Aircraft symbol
		const auto& renderAircraftSymbolRect = [&renderer](const Point& position, uint16_t width) {
			renderer->renderFilledRectangle(
				Bounds(
					position.getX(),
					position.getY(),
					width,
					PFD::aircraftSymbolThickness
				),
				&Theme::bg1
			);

			renderer->renderHorizontalLine(Point(position.getX() - 1, position.getY() - 1), width + 2, &Theme::fg1);
			renderer->renderHorizontalLine(Point(position.getX() - 1, position.getY() + PFD::aircraftSymbolThickness), width + 2, &Theme::fg1);

			renderer->renderVerticalLine(Point(position.getX() - 1, position.getY()), PFD::aircraftSymbolThickness, &Theme::fg1);
			renderer->renderVerticalLine(Point(position.getX() + width, position.getY()), PFD::aircraftSymbolThickness, &Theme::fg1);
		};

		// Left
		renderAircraftSymbolRect(
			Point(
				center.getX() - PFD::aircraftSymbolCenterOffset - PFD::aircraftSymbolThickness - PFD::aircraftSymbolWidth,
				center.getY() - PFD::aircraftSymbolThickness / 2
			),
			PFD::aircraftSymbolWidth
		);

		// Right
		renderAircraftSymbolRect(
			Point(
				center.getX() + PFD::aircraftSymbolCenterOffset + PFD::aircraftSymbolThickness,
				center.getY() - PFD::aircraftSymbolThickness / 2
			),
			PFD::aircraftSymbolWidth
		);

		// Dot
		renderAircraftSymbolRect(
			Point(
				center.getX() - PFD::aircraftSymbolThickness / 2,
				center.getY() - PFD::aircraftSymbolThickness / 2
			),
			PFD::aircraftSymbolThickness
		);
	}

	void PFDScene::renderSyntheticVisionBackground(
		Renderer* renderer,
		const Bounds& bounds,
		const Point& horizonLeft,
		const Point& horizonRight
	) {
		// Sky
		renderer->renderFilledRectangle(bounds, &Theme::sky);

		// Ground
		const auto groundMaxY = std::max(horizonLeft.getY(), horizonRight.getY());

		// Triangle
		renderer->renderFilledTriangle(
			horizonLeft.getY() < horizonRight.getY()
			? horizonLeft
			: horizonRight,
			Point(
				horizonLeft.getX(),
				groundMaxY
			),
			Point(
				horizonRight.getX(),
				groundMaxY
			),
			&Theme::ground
		);

		// Rectangle

		// Bottom
		if (groundMaxY < bounds.getY2()) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					groundMaxY,
					bounds.getWidth(),
					bounds.getY2() - groundMaxY + 1
				),
				&Theme::ground
			);
		}
		// Left
		else if (horizonLeft.getY() < bounds.getY() && horizonLeft.getX() > bounds.getX()) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY(),
					horizonLeft.getX() - bounds.getX(),
					bounds.getHeight()
				),
				&Theme::ground
			);
		}
		// Right
		else if (horizonRight.getY() < bounds.getY() && horizonRight.getX() < bounds.getX2()) {
			renderer->renderFilledRectangle(
				Bounds(
					horizonRight.getX(),
					bounds.getY(),
					bounds.getX2() - horizonRight.getX(),
					bounds.getHeight()
				),
				&Theme::ground
			);
		}
	}

	void PFDScene::renderPitchOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		const AircraftData& ad,
		float pitchPixelOffsetProjected,
		float projectionPlaneDistance,
		const Point& horizonLeft,
		const Point& horizonRight,
		const Vector2F& horizonVec,
		const Vector2F& horizonVecNorm,
		const Vector2F& horizonVecPerp,
		const Vector2F& horizonCenter
	) {
		// Middle line
		renderer->renderLine(
			horizonLeft,
			horizonRight,
			PFD::pitchOverlayColorGround
		);

		const auto viewport = renderer->pushViewport(bounds);

		for (int32_t lineAngleDeg = -90; lineAngleDeg <= 90; lineAngleDeg += PFD::pitchOverlayAngleStepDeg) {
			if (lineAngleDeg == 0)
				continue;

			const auto color = lineAngleDeg >= 0 ? PFD::pitchOverlayColorGround : PFD::pitchOverlayColorSky;

			// Same as tan(lineAngleDeg) * projectionPlaneDistance, but with spherical correction
			// This loop uses horizon as starting point, not aircraft pitch, so we just subtract it
			const auto lineCenterPerp = horizonCenter + horizonVecPerp * (std::tanf(ad.computed.pitchRad + toRadians(lineAngleDeg)) * projectionPlaneDistance - pitchPixelOffsetProjected);

			const auto lineVec = horizonVecNorm * (
				(
					lineAngleDeg % 10 == 0
						? PFD::pitchOverlayLineBig
						: PFD::pitchOverlayLineSmall
				)
				/ 2
			);

			const auto lineLeft = static_cast<Point>(lineCenterPerp - lineVec);
			const auto lineRight = static_cast<Point>(lineCenterPerp + lineVec);

			renderer->renderLine(
				lineLeft,
				lineRight,
				color
			);

			if (lineAngleDeg % 10 == 0) {
				const auto text = std::to_wstring(abs(lineAngleDeg));

				const auto& textCenterVec = Vector2F(static_cast<float>(PFD::pitchOverlayFont->getWidth(text)) / 2.f, static_cast<float>(PFD::pitchOverlayFont->getHeight()) / 2.f);
				const auto textCenterVecLengthWithOffset = static_cast<float>(PFD::pitchOverlayTextOffset) + textCenterVec.getLength();

				renderer->renderString(
					Point(
						lineRight.getX() + static_cast<int32_t>(horizonVecNorm.getX() * textCenterVecLengthWithOffset - textCenterVec.getX()),
						lineRight.getY() + static_cast<int32_t>(horizonVecNorm.getY() * textCenterVecLengthWithOffset - textCenterVec.getY())
					),
					PFD::pitchOverlayFont,
					color,
					text
				);
			}
		}

		renderer->popViewport(viewport);
	}

	void PFDScene::renderTurnCoordinatorOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		const AircraftData& aircraftData
	) {
		const auto& center = Point(
			bounds.getXCenter(),
			bounds.getY() + PFD::turnCoordinatorOverlayRollIndicatorRadius
		);

		const auto& renderLine = [&renderer, &center, &aircraftData](int8_t angle, bool isBig) {
			const auto vec = Vector2F(0, PFD::turnCoordinatorOverlayRollIndicatorRadius).rotate(toRadians(angle) - aircraftData.computed.rollRad);
			const auto lineFrom = center - static_cast<Point>(vec);

			renderer->renderLine(
				lineFrom,
				lineFrom + static_cast<Point>(vec.normalize() * (isBig
																	? PFD::turnCoordinatorOverlayRollIndicatorLineBigLength
																	: PFD::turnCoordinatorOverlayRollIndicatorLineSmallLength)),
				PFD::turnCoordinatorOverlayColor
			);
		};

		renderLine(-60, true);
		renderLine(-45, false);
		renderLine(-30, true);
		renderLine(-20, false);
		renderLine(-10, false);

		renderLine(10, false);
		renderLine(20, false);
		renderLine(30, true);
		renderLine(45, false);
		renderLine(60, true);

		// Upper triangle
		renderer->renderFilledTriangle(
			center + static_cast<Point>(Vector2F(-PFD::turnCoordinatorOverlayRollIndicatorTriangleWidth / 2,
												-PFD::turnCoordinatorOverlayRollIndicatorRadius).rotate(-aircraftData.computed.rollRad)),
			center + static_cast<Point>(Vector2F(PFD::turnCoordinatorOverlayRollIndicatorTriangleWidth / 2,
												-PFD::turnCoordinatorOverlayRollIndicatorRadius).rotate(-aircraftData.computed.rollRad)),
			center + static_cast<Point>(Vector2F(
				0, -PFD::turnCoordinatorOverlayRollIndicatorRadius + PFD::turnCoordinatorOverlayRollIndicatorTriangleHeight).rotate(
				-aircraftData.computed.rollRad)),
			PFD::turnCoordinatorOverlayColor
		);

		// Lower triangle
		const int32_t rollTriangleY = bounds.getY() + PFD::turnCoordinatorOverlayRollIndicatorTriangleHeight + PFD::turnCoordinatorOverlayRollIndicatorTriangleOffset;

		renderer->renderFilledTriangle(
			Point(center.getX(), rollTriangleY),
			Point(center.getX() - PFD::turnCoordinatorOverlayRollIndicatorTriangleWidth / 2, rollTriangleY + PFD::turnCoordinatorOverlayRollIndicatorTriangleHeight),
			Point(center.getX() + PFD::turnCoordinatorOverlayRollIndicatorTriangleWidth / 2, rollTriangleY + PFD::turnCoordinatorOverlayRollIndicatorTriangleHeight),
			PFD::turnCoordinatorOverlayColor
		);

		// Slip/skid indicator
		renderer->renderFilledRectangle(
			Bounds(
				center.getX()
					+ static_cast<int32_t>(
						static_cast<float>(PFD::turnCoordinatorOverlaySlipAndSkidIndicatorMaxValuePixels)
						* aircraftData.computed.slipAndSkidFactor
					)
					- PFD::turnCoordinatorOverlaySlipAndSkidIndicatorWidth / 2,
				rollTriangleY + PFD::turnCoordinatorOverlayRollIndicatorTriangleHeight + PFD::turnCoordinatorOverlaySlipAndSkidIndicatorOffset,
				PFD::turnCoordinatorOverlaySlipAndSkidIndicatorWidth,
				PFD::turnCoordinatorOverlaySlipAndSkidIndicatorHeight
			),
			PFD::turnCoordinatorOverlayColor
		);
	}

	void PFDScene::renderYawOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		const Settings& settings,
		const AircraftData& aircraftData
	) {
		const auto viewport = renderer->pushViewport(bounds);

		const auto centerX = bounds.getXCenter();
		const auto y2 = bounds.getY2();

		float closestInteger;
		const float closestFractional = std::modff(aircraftData.computed.headingDeg / PFD::yawOverlayAngleStepUnits, &closestInteger);
		closestInteger *= PFD::yawOverlayAngleStepUnits;

		const uint8_t fullCount = static_cast<uint8_t>(std::ceilf(static_cast<float>(centerX - bounds.getX()) / PFD::yawOverlayAngleStepPixels)) + 1;
		int32_t x = centerX - fullCount * PFD::yawOverlayAngleStepPixels - static_cast<int32_t>(closestFractional * static_cast<float>(PFD::yawOverlayAngleStepPixels));
		auto angle = static_cast<int16_t>(closestInteger - static_cast<float>(fullCount * PFD::yawOverlayAngleStepUnits));

		if (angle < 0)
			angle += 360;

		while (x <= bounds.getX2()) {
			const auto isBig = angle % PFD::yawOverlayAngleStepUnitsBig == 0;
			const auto lineLength = isBig ? PFD::yawOverlayLineBigLength : PFD::yawOverlayLineSmallLength;

			// Line
			const int32_t lineY = y2 - lineLength + 1;

			renderer->renderVerticalLine(
				Point(
					x,
					lineY
				),
				lineLength,
				PFD::yawOverlayColor
			);

			// Text
			if (isBig) {
				std::wstring text;

				switch (angle) {
					case 0:
						text = L'N';
						break;

					case 90:
						text = L'E';
						break;

					case 180:
						text = L'S';
						break;

					case 270:
						text = L'W';
						break;

					default:
						text = std::to_wstring(angle);
						break;
				}

				renderer->renderString(
					Point(
						x - PFD::yawOverlayFont->getWidth(text) / 2,
						lineY - PFD::yawOverlayTextOffset - PFD::yawOverlayFont->getHeight()
					),
					PFD::yawOverlayFont,
					PFD::yawOverlayColor,
					text
				);
			}

			x += PFD::yawOverlayAngleStepPixels;
			angle += PFD::yawOverlayAngleStepUnits;

			if (angle >= 360)
				angle -= 360;
		}
		
		// Autopilot value
		{
			auto deltaDeg = static_cast<float>(settings.autopilot.headingDeg) - aircraftData.computed.headingDeg;
			
			if (deltaDeg >= 180)
				deltaDeg -= 360;
			
			const auto deltaPixels = deltaDeg / static_cast<float>(PFD::yawOverlayAngleStepUnits) * PFD::yawOverlayAngleStepPixels;
			
			// Clamped center of indicator - indicator width
			x = std::clamp<int32_t>(
				static_cast<int32_t>(centerX + deltaPixels),
				bounds.getX(),
				bounds.getX2()
			)
			- PFD::autopilotIndicatorSize / 2;
			
			const auto y = y2 - PFD::autopilotIndicatorThickness + 1;
			
			// Rect between
			renderer->renderFilledRectangle(
				Bounds(
					x,
					y2 - PFD::autopilotIndicatorRectangleThickness + 1,
					PFD::autopilotIndicatorSize,
					PFD::autopilotIndicatorRectangleThickness
				),
				&Theme::ocean
			);
			
			// Left rect
			renderer->renderFilledRectangle(
				Bounds(
					x,
					y,
					PFD::autopilotIndicatorTriangleMargin,
					PFD::autopilotIndicatorTriangleThickness
				),
				&Theme::ocean
			);
			
			// Right rect
			renderer->renderFilledRectangle(
				Bounds(
					x + PFD::autopilotIndicatorSize - 1 - PFD::autopilotIndicatorTriangleMargin,
					y,
					PFD::autopilotIndicatorTriangleMargin,
					PFD::autopilotIndicatorTriangleThickness
				),
				&Theme::ocean
			);
			
			// Left triangle
			renderer->renderFilledTriangle(
				Point(
					x + PFD::autopilotIndicatorTriangleMargin,
					y
				),
				Point(
					x + PFD::autopilotIndicatorTriangleMargin + PFD::currentValueTriangleSize - 1,
					y + PFD::autopilotIndicatorTriangleThickness - 1
				),
				Point(
					x + PFD::autopilotIndicatorTriangleMargin,
					y + PFD::autopilotIndicatorTriangleThickness - 1
				),
				&Theme::ocean
			);
			
			// Right triangle
			renderer->renderFilledTriangle(
				Point(
					x + PFD::autopilotIndicatorSize - 1 - PFD::autopilotIndicatorTriangleMargin,
					y
				),
				Point(
					x + PFD::autopilotIndicatorSize - 1 - PFD::autopilotIndicatorTriangleMargin - PFD::currentValueTriangleSize + 1,
					y + PFD::autopilotIndicatorTriangleThickness - 1
				),
				Point(
					x + PFD::autopilotIndicatorSize - 1 - PFD::autopilotIndicatorTriangleMargin,
					y + PFD::autopilotIndicatorTriangleThickness - 1
				),
				&Theme::ocean
			);
		}
		
		// Small triangle representing current heading
		renderer->renderFilledTriangle(
			Point(centerX, y2 - PFD::yawOverlayTriangleHeight),
			Point(centerX - PFD::yawOverlayTriangleWidth / 2, y2),
			Point(centerX + PFD::yawOverlayTriangleWidth / 2, y2),
			PFD::yawOverlayColor
		);

		renderer->popViewport(viewport);
	}

	PFD::PFD() {
		setClipToBounds(true);

		_scene.setMargin(Margin(speedWidth, 0, altitudeWidth + verticalSpeedWidth, 0));
		*this += &_scene;
	}

	void PFD::onTick() {
		Element::onTick();

		auto& rc = RC::getInstance();
		const auto& ad = rc.getAircraftData();

		_scene.setCameraPosition(ad.raw.coordinates.toCartesian());

		_scene.setWorldRotation(Vector3F(
			toRadians(90) - ad.raw.coordinates.getLatitude(),
			0,
			toRadians(90) + ad.raw.coordinates.getLongitude()
		));

		_scene.setCameraRotation(Vector3F(
			ad.computed.pitchRad,
			ad.computed.rollRad,
			ad.computed.yawRad
		));

		invalidate();
	}

	void PFD::onRender(Renderer* renderer, const Bounds& bounds) {
		Layout::onRender(renderer, bounds);

		renderSpeed(renderer, Bounds(
			bounds.getX(),
			bounds.getY() + miniHeight,
			speedWidth,
			bounds.getHeight() - miniHeight * 2
		));

		renderGroundSpeed(renderer, Bounds(
			bounds.getX(),
			bounds.getY2() + 1 - miniHeight,
			speedWidth,
			miniHeight
		));

		renderAutopilotSpeed(renderer, Bounds(
			bounds.getX(),
			bounds.getY(),
			speedWidth,
			miniHeight
		));

		renderAltitude(renderer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY() + miniHeight,
			altitudeWidth,
			bounds.getHeight() - miniHeight * 2
		));

		renderAutopilotAltitude(renderer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY(),
			altitudeWidth,
			miniHeight
		));

		renderPressure(renderer, Bounds(
			bounds.getX2() + 1 - altitudeWidth - verticalSpeedWidth,
			bounds.getY2() + 1 - miniHeight,
			altitudeWidth,
			miniHeight
		));

		renderVerticalSpeed(renderer, Bounds(
			bounds.getX2() + 1 - verticalSpeedWidth,
			bounds.getY(),
			verticalSpeedWidth,
			bounds.getHeight()
		));
	}

	void PFD::renderCurrentValue(Renderer* renderer, const Bounds& bounds, uint8_t digitCount, float value, bool left) {
		const auto isConnected = RC::getInstance().getPacketHandler().isConnected();
		const auto bg = isConnected ? &Theme::bg2 : &Theme::bad1;
		const auto x2 = bounds.getX2();
		const auto yCenter = bounds.getYCenter();
		
		// Rect
		renderer->renderFilledRectangle(
			Bounds(
				left ? bounds.getX() : bounds.getX() + currentValueTriangleSize,
				bounds.getY(),
				bounds.getWidth() - currentValueTriangleSize,
				bounds.getHeight()
			),
			bg
		);
		
		// Triangle
		renderer->renderFilledTriangle(
			Point(
				left ? x2 - currentValueTriangleSize + 1 : bounds.getX() + currentValueTriangleSize - 1,
				bounds.getY()
			),
			Point(
				left ? x2 : bounds.getX(),
				yCenter
			),
			Point(
				left ? x2 - currentValueTriangleSize + 1 : bounds.getX() + currentValueTriangleSize - 1,
				bounds.getY2()
			),
			bg
		);

		// Text
		const auto textY = yCenter - currentValueFont->getHeight() / 2;
		
		// Assuming 4 is "widest" digit

		if (isConnected) {
			const auto oldViewport = renderer->pushViewport(bounds);
			
			value = std::abs(value);
			
			const uint8_t maxDigitWidth = currentValueFont->getWidth(L'4');
			
			int32_t textX =
				left
				? x2 - speedBarSize - currentValueTextOffset - maxDigitWidth
				: bounds.getX() + currentValueTextOffset + maxDigitWidth * (digitCount - 1) + 1;
			
			const auto getAdjacentDigit = [&](uint8_t digit, bool plus) {
				return
					plus
					? (digit < 9 ? digit + 1 : 0)
					: (digit > 1 ? digit - 1 : 9);
			};
			
			const auto renderDigit = [&](int32_t digitY, uint8_t digit) {
				const wchar_t text = L'0' + digit;
				
				renderer->renderChar(
					Point(
						textX,
						digitY
					),
					currentValueFont,
					&Theme::fg1,
					text
				);
			};
			
			for (uint8_t digitIndex = 0; digitIndex < digitCount; ++digitIndex) {
				const auto digit = static_cast<uint32_t>(value) % 10;
				
				// Roll
				if (digitIndex == 0) {
					float integer;
					const auto fractional = std::modf(value, &integer);
					const auto rolledY = textY + static_cast<int32_t>(fractional * static_cast<float>(currentValueFont->getHeight()));
					
					renderDigit(rolledY - currentValueFont->getHeight(), getAdjacentDigit(digit, true));
					renderDigit(rolledY, digit);
					renderDigit(rolledY + currentValueFont->getHeight(), getAdjacentDigit(digit, false));
				}
				else {
					renderDigit(textY, digit);
				}
				
				textX -= maxDigitWidth;
				value = value / 10;
			}
			
			renderer->popViewport(oldViewport);
		}
		else {
			const auto text = L"---";
			
			renderer->renderString(
				Point(
					left
						? x2 - speedBarSize - currentValueTextOffset - Theme::fontSmall.getWidth(text)
						: bounds.getX() + speedBarSize + currentValueTextOffset,
					textY
				),
				&Theme::fontSmall,
				&Theme::bad3,
				text
			);
		}
	}

	void PFD::renderVerticalAutopilotValueIndicator(Renderer* renderer, const Point& point, bool left) {
		// Upper rect
		renderer->renderFilledRectangle(
			Bounds(
				point.getX(),
				point.getY(),
				autopilotIndicatorThickness,
				autopilotIndicatorTriangleMargin
			),
			&Theme::ocean
		);

		// Lower rect
		renderer->renderFilledRectangle(
			Bounds(
				point.getX(),
				point.getY() + autopilotIndicatorSize - autopilotIndicatorTriangleMargin,
				autopilotIndicatorThickness,
				autopilotIndicatorTriangleMargin
			),
			&Theme::ocean
		);

		// Rect between
		renderer->renderFilledRectangle(
			Bounds(
				left ? point.getX() + autopilotIndicatorTriangleThickness : point.getX(),
				point.getY() + autopilotIndicatorTriangleMargin,
				autopilotIndicatorRectangleThickness,
				autopilotIndicatorTriangleSize
			),
			&Theme::ocean
		);

		// Upper triangle
		renderer->renderFilledTriangle(
			Point(
				left ? point.getX() : point.getX() + autopilotIndicatorRectangleThickness,
				point.getY() + autopilotIndicatorTriangleMargin
			),
			Point(
				left ? point.getX() + autopilotIndicatorTriangleThickness - 1 : point.getX() + autopilotIndicatorThickness - 1,
				point.getY() + autopilotIndicatorTriangleMargin
			),
			Point(
				left ? point.getX() + autopilotIndicatorTriangleThickness - 1 : point.getX() + autopilotIndicatorRectangleThickness - 1,
				point.getY() + autopilotIndicatorSize / 2
			),
			&Theme::ocean
		);

		// Lower triangle
		renderer->renderFilledTriangle(
			Point(
				left ? point.getX() + autopilotIndicatorTriangleThickness - 1 : point.getX() + autopilotIndicatorRectangleThickness,
				point.getY() + autopilotIndicatorSize / 2
			),
			Point(
				left ? point.getX() + autopilotIndicatorTriangleThickness - 1 : point.getX() + autopilotIndicatorThickness - 1,
				point.getY() + autopilotIndicatorSize - autopilotIndicatorTriangleMargin - 1
			),
			Point(
				left ? point.getX() : point.getX() + autopilotIndicatorRectangleThickness - 1,
				point.getY() + autopilotIndicatorSize - autopilotIndicatorTriangleMargin - 1
			),
			&Theme::ocean
		);
	}

	void PFD::renderAutopilotValueIndicator(Renderer* renderer, const Bounds& bounds, int32_t centerY, uint8_t unitStep, uint16_t stepPixels, float currentValue, uint16_t autopilotValue, bool left) {
		if (autopilotValue == 0)
			return;
		
		const auto indicatorCenterY = std::clamp<int32_t>(
			centerY
				- static_cast<int32_t>(
					(static_cast<float>(autopilotValue) - currentValue)
					* static_cast<float>(stepPixels)
					/ static_cast<float>(unitStep)
				),
			bounds.getY(),
			bounds.getY2()
		);
		
		renderVerticalAutopilotValueIndicator(
			renderer,
			Point(
				left ? bounds.getX2() + 1 - autopilotIndicatorThickness : bounds.getX(),
				indicatorCenterY - autopilotIndicatorSize / 2
			),
			left
		);
	}

	void PFD::renderTrendArrow(Renderer* renderer, int32_t x, int32_t y, uint8_t unitStep, uint16_t stepPixels, float value) {
		const auto length = static_cast<int32_t>(static_cast<float>(stepPixels) * value / static_cast<float>(unitStep));

		if (std::abs(length) < unitStep)
			return;

		constexpr uint8_t arrowSize = 3;

		const auto yArrow = y - length - arrowSize;
		const auto yMin = std::min(y, yArrow);
		const auto yMax = std::max(y, yArrow);

		renderer->renderVerticalLine(
			Point(x, yMin),
			yMax - yMin,
			&Theme::purple
		);

		renderer->renderFilledTriangle(
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

	void PFD::renderSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		const auto& ad = rc.getAircraftData();

		const auto centerY = bounds.getYCenter();

		renderer->renderFilledRectangle(bounds, &Theme::bg1);

		// Bars
		const auto barX = bounds.getX2() + 1 - speedBarSize;

		const auto renderBar = [&](int32_t x, uint16_t width, uint16_t fromSpeed, uint16_t toSpeed, const Color* color) {
			const int32_t fromY = centerY - static_cast<int32_t>((static_cast<float>(fromSpeed) - ad.computed.airspeedKt) * static_cast<float>(speedStepPixels) / static_cast<float>(speedStepUnits));
			const int32_t height = (toSpeed - fromSpeed) * speedStepPixels / speedStepUnits;

			renderer->renderFilledRectangle(
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
			barX,
			speedBarSize,
			0,
			speedFlapsMin,
			&Theme::red
		);

		renderBar(
			barX,
			speedBarSize,
			speedFlapsMin,
			speedFlapsMax,
			&Theme::fg1
		);

		renderBar(
			barX,
			speedBarSize,
			speedSmoothMin,
			speedSmoothMax,
			&Theme::greenSpeed
		);

		renderBar(
			barX,
			speedBarSize,
			speedTurbulentMin,
			speedTurbulentMax,
			&Theme::yellow
		);

		renderBar(
			barX,
			speedBarSize,
			speedStructuralMin,
			speedStructuralMax,
			&Theme::red
		);

		// Lines
		const float snapped = ad.computed.airspeedKt / static_cast<float>(speedStepUnits);
		const float snappedInteger = std::floorf(snapped);
		const float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - static_cast<uint16_t>((1.0f - snappedFractional) * static_cast<float>(speedStepPixels));
		const auto altitudeYFullLines = static_cast<int32_t>(std::ceil(static_cast<float>(y) / static_cast<float>(speedStepPixels)));
		y = y - altitudeYFullLines * speedStepPixels;

		int32_t lineValue = static_cast<int32_t>(snappedInteger + 1) * speedStepUnits + altitudeYFullLines * speedStepUnits;

		const Color* lineColor = &Theme::fg3;

		do {
			const auto isBig = lineValue % speedStepUnitsBig == 0;

			if (isBig) {
				// Line
				renderer->renderHorizontalLine(
					Point(
						bounds.getX2() + 1 - speedBarSize - lineSizeBig,
						y
					),
					lineSizeBig,
					lineColor
				);

				// Text
				const auto& text = std::to_wstring(lineValue);

				renderer->renderString(
					Point(
						bounds.getX2() + 1 - speedBarSize - lineSizeBig - lineTextOffset - currentValueFont->getWidth(text),
						y - currentValueFont->getHeight() / 2
					),
					currentValueFont,
					lineColor,
					text
				);
			}
			else {
				// Line
				renderer->renderHorizontalLine(
					Point(bounds.getX2() + 1 - speedBarSize - lineSizeSmall, y),
					lineSizeSmall,
					lineColor
				);
			}

			lineValue -= speedStepUnits;
			y += speedStepPixels;
		} while (y <= bounds.getY2() && lineValue >= 0);

		// Trend
		renderTrendArrow(
			renderer,
			bounds.getX2() - speedBarSize - lineSizeBig,
			centerY,
			speedStepUnits,
			speedStepPixels,
			ad.computed.airspeedTrendKt
		);

		// V-speeds
		for (const auto& bug : speedBugs) {
			const auto& bugBounds = Bounds(
				bounds.getX2() + 1 + speedBugOffset + speedBugTriangleWidth,
				centerY - static_cast<int32_t>((static_cast<float>(bug.getValue()) - ad.computed.airspeedKt) * static_cast<float>(speedStepPixels) / static_cast<float>(speedStepUnits)),
				Theme::fontSmall.getWidth(bug.getName()) + speedBugTextOffset * 2,
				Theme::fontSmall.getHeight() + speedBugTextOffset * 2
			);

			// Rect
			renderer->renderFilledRectangle(
				bugBounds,
				1,
				&Theme::bg2
			);

			// Triangle
			renderer->renderFilledTriangle(
				Point(bugBounds.getX(), bugBounds.getY()),
				Point(bugBounds.getX(), bugBounds.getY2()),
				Point(bugBounds.getX() - speedBugTriangleWidth, bugBounds.getYCenter()),
				&Theme::bg2
			);

			// Text
			renderer->renderString(
				Point(bugBounds.getX() + speedBugTextOffset, bugBounds.getY() + speedBugTextOffset),
				&Theme::fontSmall,
				&Theme::green,
				bug.getName()
			);
		}

		// Autopilot
		renderAutopilotValueIndicator(
			renderer,
			bounds,
			centerY,
			speedStepUnits,
			speedStepPixels,
			ad.computed.airspeedKt,
			rc.getSettings().autopilot.speedKt,
			true
		);

		// Current speed
		renderCurrentValue(
			renderer,
			Bounds(
				bounds.getX(),
				centerY - currentValueHeight / 2,
				bounds.getWidth(),
				currentValueHeight
			),
			speedMaximumDigits,
			ad.computed.airspeedKt,
			true
		);
	}

	void PFD::renderAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		const auto& ad = rc.getAircraftData();

		const auto centerY = bounds.getYCenter();
		const auto x = bounds.getX();

		renderer->renderFilledRectangle(bounds, &Theme::bg1);

		const float altitude = ad.computed.altitudeFt;
		const float snapped = altitude / static_cast<float>(altitudeStepUnits);
		const float snappedInteger = std::floorf(snapped);
		const float snappedFractional = snapped - snappedInteger;

		int32_t y = centerY - static_cast<uint16_t>((1.0f - snappedFractional) * static_cast<float>(altitudeStepPixels));
		const auto yFullLines = static_cast<int32_t>(std::ceilf(static_cast<float>(y) / static_cast<float>(altitudeStepPixels)));
		y = y - yFullLines * altitudeStepPixels;

		int32_t lineValue = static_cast<int32_t>(snappedInteger + 1) * altitudeStepUnits + yFullLines * altitudeStepUnits;

		const Color* lineColor = &Theme::fg3;

		do {
			const auto isBig = lineValue % altitudeStepUnitsBig == 0;

			if (isBig) {
				renderer->renderHorizontalLine(
					Point(x, y),
					lineSizeBig,
					lineColor
				);

				// Text
				renderer->renderString(
					Point(x + lineSizeBig + lineTextOffset, y - currentValueFont->getHeight() / 2),
					currentValueFont,
					lineColor,
					std::to_wstring(lineValue)
				);
			}
			else {
				renderer->renderHorizontalLine(
					Point(x, y),
					lineSizeSmall,
					lineColor
				);
			}

			lineValue -= altitudeStepUnits;
			y += altitudeStepPixels;
		} while (y < bounds.getY2() && lineValue >= 0);

		// Ground
		if (y < bounds.getY2() && lineValue < 0) {
//			renderer->renderHorizontalLine(
//				Point(x, y),
//				bounds.getWidth(),
//				&Theme::yellow
//			);
//
			constexpr int8_t groundSpacing = 5;
			auto groundPoint1 = Point(x, y + groundSpacing);
			auto groundPoint2 = Point(x + groundSpacing, y);
		
			do {
				renderer->renderLine(
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
			renderer,
			bounds.getX() + lineSizeBig,
			centerY,
			altitudeStepUnits,
			altitudeStepPixels,
			ad.computed.altitudeTrendFt
		);

		// Minimums
		if (rc.getSettings().controls.minimumAltitudeEnabled) {
			const int32_t delta = altitude - static_cast<float>(rc.getSettings().controls.minimumAltitudeFt);

			y = centerY + static_cast<int32_t>(delta * static_cast<float>(altitudeStepPixels) / static_cast<float>(altitudeStepUnits));

			const auto& linePosition = Point(bounds.getX() - altitudeMinimumHorizontalOffset + altitudeMinimumTriangleWidth, y);

			const Color* color;

			if (std::abs(delta) <= altitudeMinimumSafeUnitDelta) {
				color = &Theme::fg1;
			}
			else {
				color = &Theme::yellow;
			}

			// Line
			renderer->renderHorizontalLine(
				linePosition,
				bounds.getWidth() + altitudeMinimumHorizontalOffset - altitudeMinimumTriangleWidth,
				color
			);

			// Triangle
			renderer->renderFilledTriangle(
				linePosition,
				Point(linePosition.getX() - altitudeMinimumTriangleWidth, linePosition.getY() - altitudeMinimumTriangleHeight),
				Point(linePosition.getX() - altitudeMinimumTriangleWidth, linePosition.getY() + altitudeMinimumTriangleHeight),
				color
			);
		}

		// Autopilot
		renderAutopilotValueIndicator(
			renderer,
			bounds,
			centerY,
			altitudeStepUnits,
			altitudeStepPixels,
			altitude,
			rc.getSettings().autopilot.altitudeFt,
			false
		);

		// Current value
		renderCurrentValue(
			renderer,
			Bounds(
				bounds.getX(),
				centerY - currentValueHeight / 2,
				bounds.getWidth(),
				currentValueHeight
			),
			altitudeMaximumDigits,
			altitude,
			false
		);
	}

	void PFD::renderVerticalSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		const auto& ad = rc.getAircraftData();

		const auto centerY = bounds.getYCenter();

		// Background
		renderer->renderFilledRectangle(bounds, &Theme::bg2);

		// Lines
		auto lineColor = &Theme::fg4;

		int32_t y = centerY;
		int32_t lineValue = 0;

		bool isBig;

		auto renderLines = [&lineValue, &isBig, &renderer, lineColor, &bounds, &y](int32_t yAdder) {
			while (lineValue <= verticalSpeedStepUnitsLimit) {
				isBig = lineValue % verticalSpeedStepUnitsBig == 0;

				if (isBig) {
					renderer->renderHorizontalLine(
						Point(bounds.getX(), y),
						verticalSpeedLineSizeBig,
						lineColor
					);

					renderer->renderString(
						Point(
							bounds.getX() + verticalSpeedLineSizeBig + verticalSpeedLineTextOffset,
							y - verticalSpeedFont->getHeight() / 2
						),
						verticalSpeedFont,
						lineColor,
						std::to_wstring(lineValue / 1000)
					);
				}
				else {
					renderer->renderHorizontalLine(
						Point(bounds.getX(), y),
						verticalSpeedLineSizeSmall,
						lineColor
					);
				}

				y += yAdder;
				lineValue += verticalSpeedStepUnits;
			}
		};

		renderLines(-verticalSpeedStepPixels);

		y = centerY + verticalSpeedStepPixels;
		lineValue = verticalSpeedStepUnits;
		renderLines(verticalSpeedStepPixels);

		// Current value
		renderer->renderLine(
			Point(bounds.getX(), centerY - static_cast<int32_t>(ad.computed.verticalSpeedFtPM * static_cast<float>(verticalSpeedStepPixels) / static_cast<float>(verticalSpeedStepUnits))),
			Point(bounds.getX2(), centerY - static_cast<int32_t>(ad.computed.verticalSpeedFtPM * static_cast<float>(verticalSpeedStepPixelsRight) / static_cast<float>(verticalSpeedStepUnits))),
			&Theme::green
		);
	}

	void PFD::renderMiniPanel(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, std::wstring_view text, int8_t textXOffset) {
		// Background
		renderer->renderFilledRectangle(bounds, bg);

		// Text
		renderer->renderString(
			Point(
				bounds.getX() + textXOffset + (bounds.getWidth() - textXOffset) / 2 - miniFont->getWidth(text) / 2,
				bounds.getY() + miniHeight / 2 - miniFont->getHeight() / 2
			),
			miniFont,
			fg,
			text
		);
	}

	void PFD::renderMiniPanelWithAutopilotValue(Renderer* renderer, const Bounds& bounds, const Color* bg, const Color* fg, uint16_t autopilotValue, bool autopilotValueEnabled, bool left) {
		renderMiniPanel(
			renderer,
			bounds,
			bg,
			fg,
			autopilotValueEnabled ? std::to_wstring(autopilotValue) : L"----",
			static_cast<int8_t>(left ? -autopilotIndicatorTriangleThickness : autopilotIndicatorTriangleThickness)
		);
	}

	void PFD::renderAutopilotSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		const auto bg = &Theme::bg2;
		const auto fg = &Theme::ocean;

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.speedKt, rc.getSettings().autopilot.speedKt > 0, true);
	}

	void PFD::renderAutopilotAltitude(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		const auto bg = &Theme::bg2;
		const auto fg = &Theme::ocean;

		renderMiniPanelWithAutopilotValue(renderer, bounds, bg, fg, rc.getSettings().autopilot.altitudeFt, rc.getSettings().autopilot.altitudeFt > 0, false);
	}

	void PFD::renderPressure(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		auto bg = &Theme::bg2;
		auto fg = &Theme::yellow;

		std::wstring text;

		if (rc.getSettings().controls.referencePressureSTD) {
			text = L"STD";
			bg = &Theme::yellow;
			fg = &Theme::bg1;
		}
		else {
			text = std::to_wstring(static_cast<uint32_t>(
				Units::convertPressure(
					rc.getSettings().controls.referencePressurePa,
					PressureUnit::pascal,
					PressureUnit::hectopascal
				)
			));
		}

		renderMiniPanel(renderer, bounds, bg, fg, text, 0);
	}

	void PFD::renderGroundSpeed(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		const auto& ad = rc.getAircraftData();

		renderMiniPanel(renderer, bounds, &Theme::bg2, &Theme::purple, std::to_wstring(static_cast<uint16_t>(ad.raw.groundSpeedKt)), 0);
	}
}