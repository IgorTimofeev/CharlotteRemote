#include "PFDScene.h"

#include <numbers>

#include "rc.h"
#include "UI/theme.h"
#include "UI/navigation/MFD/PFD/PFD.h"
#include "UI/navigation/MFD/PFD/PFDRunwayElement.h"

namespace pizda {
	PFDScene::PFDScene() {
		setClipToBounds(true);
		
		auto& rc = RC::getInstance();

		setFOV(toRadians(rc.getSettings().personalization.MFD.PFD.FOV));

		// Runways
		for (uint16_t i = 0; i < rc.getNavigationData().runways.size(); ++i) {
			addElement(new PFDRunwayElement(i));
		}
	}

	void PFDScene::onRender(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();
		
		const auto& center = bounds.getCenter();

		const auto projectionPlaneDistance = getProjectionPlaneDistance();

		const auto pitchPixelOffsetProjected = std::tanf(rc.getAircraftData().computed.pitchRad) * projectionPlaneDistance;
		const auto& pitchPixelOffsetRotated = Vector2F(0, pitchPixelOffsetProjected).rotate(-rc.getAircraftData().computed.rollRad);
		const auto diagonal = std::sqrtf(bounds.getWidth() * bounds.getWidth() + bounds.getHeight() * bounds.getHeight());
		const auto& horizonRollRotated = Vector2F(diagonal / 2.f, 0).rotate(-rc.getAircraftData().computed.rollRad);

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

		// SVT background
		{
			if (horizonLeft.getY() >= bounds.getY2() && horizonRight.getY() >= bounds.getY2()) {
				renderer->renderFilledRectangle(bounds, &Theme::sky1);
			}
			else if (horizonLeft.getY() <= bounds.getY() && horizonRight.getY() <= bounds.getY()) {
				renderer->renderFilledRectangle(bounds, &Theme::ground1);
			}
			else {
				renderer->renderFilledQuad(
					horizonLeft - static_cast<Point>(horizonVecPerp * diagonal),
					horizonRight - static_cast<Point>(horizonVecPerp * diagonal),
					horizonRight,
					horizonLeft,
					&Theme::sky1
				);

				// Ground
				renderer->renderFilledQuad(
					horizonLeft,
					horizonRight,
					horizonRight + static_cast<Point>(horizonVecPerp * diagonal),
					horizonLeft + static_cast<Point>(horizonVecPerp * diagonal),
					&Theme::ground1
				);
			}
		}

		// SVT scene
		Scene::onRender(renderer, bounds);
		
		// FMA
		const auto FMAWidth = bounds.getWidth() - PFD::flightModeAnnunciatorHorizontalOffset * 2;
		
		renderFlightModeAnnunciatorOverlay(
			renderer,
			Bounds(
				bounds.getXCenter() - FMAWidth / 2,
				bounds.getY() + PFD::flightModeAnnunciatorTopOffset,
				FMAWidth,
				PFD::flightModeAnnunciatorHeight
			)
		);
		
		// Turn coordinator
		renderTurnCoordinatorOverlay(
			renderer,
			Bounds(
				bounds.getX(),
				bounds.getY() + PFD::flightModeAnnunciatorTopOffset + PFD::flightModeAnnunciatorHeight + PFD::turnCoordinatorOverlayTopOffset,
				bounds.getWidth(),
				bounds.getHeight() - PFD::flightModeAnnunciatorTopOffset - PFD::flightModeAnnunciatorHeight - PFD::turnCoordinatorOverlayTopOffset
			)
		);

		// Pitch overlay
		renderPitchOverlay(
			renderer,
			Bounds(
				bounds.getX(),
				bounds.getY() + PFD::flightModeAnnunciatorTopOffset + PFD::flightModeAnnunciatorHeight + PFD::turnCoordinatorOverlayTopOffset + PFD::pitchOverlayVerticalOffset,
				bounds.getWidth(),
				bounds.getHeight() - (PFD::flightModeAnnunciatorTopOffset + PFD::flightModeAnnunciatorHeight + PFD::turnCoordinatorOverlayTopOffset + PFD::pitchOverlayVerticalOffset + PFD::yawOverlayHeight)
			),
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
		renderYawOverlay(
			renderer,
			Bounds(
				bounds.getX(),
				bounds.getY2() - PFD::yawOverlayHeight + 1,
				bounds.getWidth(),
				PFD::yawOverlayHeight
			)
		);

		// Wind
		if (rc.getAircraftData().raw.groundSpeedKt > PFD::windVisibilityGroundSpeed) {
			const auto& windPosition = Point(
				bounds.getX() + 6,
				bounds.getY2() - 18
			);

			constexpr uint8_t textOffset = 4;
			constexpr uint8_t arrowSize = 16;

			const auto text = std::to_wstring(static_cast<uint16_t>(rc.getAircraftData().raw.windSpeed));

			const auto arrowVec = Vector2F(0, arrowSize).rotate(rc.getAircraftData().computed.windDirectionRad + std::numbers::pi_v<float> + rc.getAircraftData().computed.yawRad);
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
		if (rc.getSettings().personalization.MFD.PFD.flightDirector) {
			const uint16_t flightDirectorLength = static_cast<uint32_t>(std::min(bounds.getWidth(), bounds.getHeight())) * PFD::flightDirectorLengthFactor / 100;
			const auto flightDirectorLengthHalfF = static_cast<float>(flightDirectorLength) / 2.f;

			// Horizontal
			auto flightDirectorRectBounds = Bounds(
				center.getX() - flightDirectorLength / 2,
				center.getY()
					- static_cast<int32_t>(std::clamp(
						std::tanf(rc.getAircraftData().computed.autopilot.pitchRad - rc.getAircraftData().computed.pitchRad) * projectionPlaneDistance,
						-flightDirectorLengthHalfF,
						flightDirectorLengthHalfF
					))
					- PFD::flightDirectorThickness / 2,
				flightDirectorLength,
				PFD::flightDirectorThickness
			);

			renderer->renderFilledRectangle(flightDirectorRectBounds, &Theme::magenta1);

			// Vertical
			flightDirectorRectBounds.setX(
				center.getX()
					+ static_cast<int32_t>(std::clamp(
						(rc.getAircraftData().computed.autopilot.rollRad - rc.getAircraftData().computed.rollRad) / toRadians(30) * flightDirectorLengthHalfF,
						-flightDirectorLengthHalfF,
						flightDirectorLengthHalfF
					))
					- PFD::flightDirectorThickness / 2
			);

			flightDirectorRectBounds.setY(center.getY() - flightDirectorLength / 2);
			flightDirectorRectBounds.setWidth(PFD::flightDirectorThickness);
			flightDirectorRectBounds.setHeight(flightDirectorLength);

			renderer->renderFilledRectangle(flightDirectorRectBounds, &Theme::magenta1);
		}

		// Flight path vector
		if (rc.getAircraftData().computed.airspeedKt > PFD::speedFlapsMin) {
			const auto& FPVPosition = Point(
				static_cast<int32_t>(horizonCenter.getX() + std::tanf(rc.getAircraftData().computed.flightPathVectorYawRad) * projectionPlaneDistance),
				static_cast<int32_t>(horizonCenter.getY() - std::tanf(rc.getAircraftData().computed.flightPathVectorPitchRad) * projectionPlaneDistance)
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
		const auto& renderAircraftSymbolRect = [&renderer](const Point& position, const uint16_t width) {
			renderer->renderFilledRectangle(
				Bounds(
					position.getX(),
					position.getY(),
					width,
					PFD::aircraftSymbolThickness
				),
				&Theme::bg1
			);

			// Outline
			// renderer->renderHorizontalLine(Point(position.getX() - 1, position.getY() - 1), width + 2, &Theme::fg1);
			// renderer->renderHorizontalLine(Point(position.getX() - 1, position.getY() + PFD::aircraftSymbolThickness), width + 2, &Theme::fg1);
			//
			// renderer->renderVerticalLine(Point(position.getX() - 1, position.getY()), PFD::aircraftSymbolThickness, &Theme::fg1);
			// renderer->renderVerticalLine(Point(position.getX() + width, position.getY()), PFD::aircraftSymbolThickness, &Theme::fg1);
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

	void PFDScene::renderPitchOverlay(
		Renderer* renderer,
		const Bounds& bounds,
		const float pitchPixelOffsetProjected,
		const float projectionPlaneDistance,
		const Point& horizonLeft,
		const Point& horizonRight,
		const Vector2F& horizonVec,
		const Vector2F& horizonVecNorm,
		const Vector2F& horizonVecPerp,
		const Vector2F& horizonCenter
	) {
		auto& rc = RC::getInstance();

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
			const auto lineCenterPerp = horizonCenter + horizonVecPerp * (std::tanf(rc.getAircraftData().computed.pitchRad + toRadians(lineAngleDeg)) * projectionPlaneDistance - pitchPixelOffsetProjected);

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
		const Bounds& bounds
	) {
		auto& rc = RC::getInstance();

		const auto& center = Point(
			bounds.getXCenter(),
			bounds.getY() + PFD::turnCoordinatorOverlayRollIndicatorRadius
		);
		const auto enoughSpace = bounds.getHeight() >= 80;

		if (enoughSpace) {
			const auto& renderLine = [&renderer, &center, &rc](const int8_t angle, const bool isBig) {
				const auto vec = Vector2F(0, PFD::turnCoordinatorOverlayRollIndicatorRadius).rotate(toRadians(angle) - rc.getAircraftData().computed.rollRad);
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
		}

		// Upper triangle
		renderer->renderFilledTriangle(
			center + static_cast<Point>(Vector2F(
				-PFD::turnCoordinatorOverlayRollIndicatorTriangleWidth / 2,
				-PFD::turnCoordinatorOverlayRollIndicatorRadius).rotate(-rc.getAircraftData().computed.rollRad)
			),
			center + static_cast<Point>(Vector2F(
				PFD::turnCoordinatorOverlayRollIndicatorTriangleWidth / 2,
				-PFD::turnCoordinatorOverlayRollIndicatorRadius).rotate(-rc.getAircraftData().computed.rollRad)
			),
			center + static_cast<Point>(Vector2F(
				0, -PFD::turnCoordinatorOverlayRollIndicatorRadius + PFD::turnCoordinatorOverlayRollIndicatorTriangleHeight)
				.rotate(-rc.getAircraftData().computed.rollRad)
			),
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
						* rc.getAircraftData().computed.slipAndSkidFactor
					)
					- PFD::turnCoordinatorOverlaySlipAndSkidIndicatorWidth / 2,
				rollTriangleY + PFD::turnCoordinatorOverlayRollIndicatorTriangleHeight + PFD::turnCoordinatorOverlaySlipAndSkidIndicatorOffset,
				PFD::turnCoordinatorOverlaySlipAndSkidIndicatorWidth,
				PFD::turnCoordinatorOverlaySlipAndSkidIndicatorHeight
			),
			PFD::turnCoordinatorOverlayColor
		);
	}
	
	void PFDScene::renderFlightModeAnnunciatorOverlay(Renderer* renderer, const Bounds& bounds) {
		auto& rc = RC::getInstance();

		constexpr static uint8_t sectionCount = 3;
		const auto sectionWidth = bounds.getWidth() / sectionCount;
		const auto yCenter = bounds.getYCenter();
		auto x = bounds.getX();
		
		const auto renderText = [renderer, yCenter, &x, sectionWidth](const std::wstring_view text, const bool ap) {
			renderer->renderString(
				Point(
					x + sectionWidth / 2 - Theme::fontSmall.getWidth(text) / 2,
					yCenter - Theme::fontSmall.getHeight() / 2
				),
				&Theme::fontSmall,
				ap ? &Theme::green1 : &Theme::sky2,
				text
			);
			
			x += sectionWidth;
		};
		
		const auto renderMissingText = [&renderText] {
			renderText(L"---", false);
		};
		
		const auto renderSeparator = [renderer, &x, &bounds] {
			renderer->renderVerticalLine(Point(x - 1, bounds.getY()), PFD::flightModeAnnunciatorHeight, &Theme::sky2);
		};
		
		// Throttle
		if (rc.getTransceiver().isConnected()) {
			if (rc.getAircraftData().raw.autopilot.autothrottle) {
				renderText(L"A/T", true);
			}
			else {
				renderText(L"MAN", false);
			}
		}
		else {
			renderMissingText();
		}
		
		renderSeparator();
		
		// Lateral
		if (rc.getTransceiver().isConnected()) {
			switch (rc.getAircraftData().raw.autopilot.lateralMode) {
				case AutopilotLateralMode::dir: {
					renderText(L"DIR", false);
					break;
				}
				case AutopilotLateralMode::stab: {
					renderText(L"STB", true);
					break;
				}
				case AutopilotLateralMode::hdg: {
					renderText(L"HDG", true);
					break;
				}
			}
		}
		else {
			renderMissingText();
		}
		
		renderSeparator();
		
		// Vertical
		if (rc.getTransceiver().isConnected()) {
			switch (rc.getAircraftData().raw.autopilot.verticalMode) {
				case AutopilotVerticalMode::dir: {
					renderText(L"DIR", false);
					break;
				}
				case AutopilotVerticalMode::stab: {
					renderText(L"STB", true);
					break;
				}
				case AutopilotVerticalMode::alt: {
					renderText(L"ALT", true);
					break;
				}
				case AutopilotVerticalMode::alts: {
					renderText(L"ALTS", true);
					break;
				}
				case AutopilotVerticalMode::flc: {
					renderText(L"FLC", true);
					break;
				}
			}
		}
		else {
			renderMissingText();
		}
	}
	
	void PFDScene::renderYawOverlay(
		Renderer* renderer,
		const Bounds& bounds
	) {
		auto& rc = RC::getInstance();
		const auto viewport = renderer->pushViewport(bounds);

		const auto centerX = bounds.getXCenter();
		const auto y2 = bounds.getY2();

		float closestInteger;
		const float closestFractional = std::modff(rc.getAircraftData().computed.headingDeg / PFD::yawOverlayAngleStepUnits, &closestInteger);
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
			auto deltaDeg =
				static_cast<int32_t>(rc.getSettings().autopilot.headingDeg)
				- static_cast<int32_t>(rc.getAircraftData().computed.headingDeg);

			if (deltaDeg > 180) {
				deltaDeg -= 360;
			}
			else if (deltaDeg < -180) {
				deltaDeg += 360;
			}

			const auto deltaPixels = deltaDeg * PFD::yawOverlayAngleStepPixels / PFD::yawOverlayAngleStepUnits;

			const auto indicatorXCenter = std::clamp<int32_t>(
				centerX + deltaPixels,
				bounds.getX(),
				bounds.getX2()
			);
			
			x = indicatorXCenter - PFD::autopilotIndicatorSize / 2;
			
			const auto y = y2 - PFD::autopilotIndicatorThickness + 1;
			
			// Lower rect
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
					x + PFD::autopilotIndicatorSize - PFD::autopilotIndicatorTriangleMargin,
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
					indicatorXCenter,
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
					indicatorXCenter,
					y + PFD::autopilotIndicatorTriangleThickness - 1
				),
				Point(
					x + PFD::autopilotIndicatorSize - 1 - PFD::autopilotIndicatorTriangleMargin,
					y + PFD::autopilotIndicatorTriangleThickness - 1
				),
				&Theme::ocean
			);
		}

		// Trend
		{
			auto deltaPixels =
				static_cast<int32_t>(rc.getAircraftData().computed.yawTrendDeg)
				* PFD::yawOverlayAngleStepPixels
				/ static_cast<float>(PFD::yawOverlayAngleStepUnits);

			x = centerX;

			if (deltaPixels < 0) {
				deltaPixels = -deltaPixels;
				x -= deltaPixels;
			}

			renderer->renderHorizontalLine(
				Point(x, y2),
				deltaPixels,
				&Theme::magenta1
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
}