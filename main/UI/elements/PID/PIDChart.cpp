#include <PIDController.h>

#include "UI/elements/PID/PIDChart.h"
#include "UI/theme.h"

namespace pizda {
	void PIDChart::onEvent(Event* event) {
		Control::onEvent(event);

		if (event->getTypeID() == PointerDownEvent::typeID) {
			updateSetpointFromPointerEvent(reinterpret_cast<PointerDownEvent*>(event)->getPosition().getY());

			setCaptured(true);

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			updateSetpointFromPointerEvent(reinterpret_cast<PointerDragEvent*>(event)->getPosition().getY());

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			setCaptured(false);

			event->setHandled(true);
		}
	}

	void PIDChart::onRender(Renderer* renderer, const Bounds& bounds) {
		constexpr static uint8_t textHOffset = 4;
		constexpr static uint8_t textVOffset = 1;

		// Background
		renderer->renderFilledRectangle(
			bounds,
			&Theme::bg2
		);

		// Axes
		renderer->renderHorizontalLine(
			bounds.getBottomLeft(),
			bounds.getWidth(),
			&Theme::fg4
		);

		renderer->renderVerticalLine(
			bounds.getTopLeft(),
			bounds.getHeight() - 1,
			&Theme::fg4
		);

		// ----------------------------- Setpoint -----------------------------

		{
			const auto y = bounds.getY2() - _setpoint * bounds.getHeight() / 100;

			// Text
			const auto text = std::to_wstring(_setpoint);

			renderer->renderString(
				Point(
					bounds.getX() + textHOffset,
					y - Theme::fontSmall.getHeight() / 2
				),
				&Theme::fontSmall,
				&Theme::fg4,
				text
			);

			// Line
			const auto lineOffset = textHOffset + Theme::fontSmall.getWidth(text) + textHOffset;

			renderer->renderHorizontalLine(
				Point(
					bounds.getX() + lineOffset,
					y
				),
				bounds.getWidth() - lineOffset,
				&Theme::fg4
			);
		}

		// ----------------------------- Chart -----------------------------

		float value  = 0;
		PIDController PIDController {};
		auto oldPoint = bounds.getBottomLeft();

		for (int32_t localX = 0; localX < bounds.getWidth(); localX += _step) {
			value = PIDController.tick(
				value,
				_setpoint,

				_PIDCoefficients.p,
				_PIDCoefficients.i,
				_PIDCoefficients.d,

				0.f,
				100.f,

				_deltaTime
			);

			Point newPoint {
				bounds.getX() + localX,
				bounds.getY2() - static_cast<int32_t>(value / 100.f * bounds.getHeight())
			};

			renderer->renderLine(
				oldPoint,
				newPoint,
				&Theme::accent1
			);

			oldPoint = newPoint;
		}
	}

	void PIDChart::updateSetpointFromPointerEvent(const int32_t pointerY) {
		const auto bounds = getBounds();
		_setpoint = 100 - std::clamp<int32_t>(pointerY - bounds.getY(), 0, bounds.getHeight()) * 100 / bounds.getHeight();

		invalidate();
	}

	void PIDChart::setPIDCoefficients(const PIDCoefficients& coefficients) {
		_PIDCoefficients = coefficients;

		invalidate();
	}

	void PIDChart::setStep(const uint64_t interval) {
		_step = interval;

		invalidate();
	}

	void PIDChart::setDeltaTime(const float deltaTime) {
		_deltaTime = deltaTime;
	}
}
