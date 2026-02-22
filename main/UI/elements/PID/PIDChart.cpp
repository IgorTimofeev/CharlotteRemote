#include <utility>
#include <format>

#include <PIDController.h>

#include "UI/elements/PID/PIDChart.h"
#include "UI/theme.h"
#include "utilities/string.h"

namespace pizda {
	PIDChart::PIDChart() {
		setBackgroundColor(Theme::bg2);
	}

	void PIDChart::onEvent(Event& event) {
		Control::onEvent(event);

		if (event.getTypeID() == PointerDownEvent::typeID) {
			updateValueFactorFromPointerEvent(reinterpret_cast<PointerDownEvent&>(event).getPosition().getY());

			setCaptured(true);

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerDragEvent::typeID) {
			updateValueFactorFromPointerEvent(reinterpret_cast<PointerDragEvent&>(event).getPosition().getY());

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerUpEvent::typeID) {
			setCaptured(false);

			event.setHandled(true);
		}
	}

	void PIDChart::onRender(Renderer& renderer, const Bounds& bounds) {
		constexpr static uint8_t textHOffset = 4;

		// Background
		renderer.renderFilledRectangle(
			bounds,
			getBackgroundColor()
		);

		// Axes
		renderer.renderHorizontalLine(
			bounds.getBottomLeft(),
			bounds.getWidth(),
			Theme::fg4
		);

		renderer.renderVerticalLine(
			bounds.getTopLeft(),
			bounds.getHeight() - 1,
			Theme::fg4
		);

		// ----------------------------- Value -----------------------------

		const auto valueTarget = _valueFactor * _valueMax;

		{
			const auto y = bounds.getY2() - static_cast<int32_t>(_valueFactor * bounds.getHeight());

			// Text
			const auto text = std::to_wstring(valueTarget);

			renderer.renderText(
				Point(
					bounds.getX() + textHOffset,
					y - Theme::fontSmall.getHeight() / 2
				),
				Theme::fontSmall,
				Theme::fg4,
				text
			);

			// Line
			const auto lineOffset = textHOffset + Theme::fontSmall.getWidth(text) + textHOffset;

			renderer.renderHorizontalLine(
				Point(
					bounds.getX() + lineOffset,
					y
				),
				bounds.getWidth() - lineOffset,
				Theme::fg4
			);
		}

		// ----------------------------- Chart -----------------------------

		std::array<Point, 100> splinePoints {};
		splinePoints[0] = bounds.getBottomLeft();
		splinePoints[1] = splinePoints[0];

		uint16_t localX = 0;
		const auto stepPixels = bounds.getWidth() / (_stepCount - 1);

		PIDController PIDController {};

		float outputValue = 0;

		for (uint16_t i = 0; i < _stepCount; ++i) {
			outputValue = PIDController.tick(
				outputValue,
				valueTarget,

				_coefficients.p,
				_coefficients.i,
				_coefficients.d,

				_deltaTime,

				0.f,
				_valueMax
			);

			localX += stepPixels;

			splinePoints[i + 2] = {
				bounds.getX() + localX,
				bounds.getY2() - static_cast<int32_t>(outputValue / _valueMax * bounds.getHeight())
			};
		}

		splinePoints[_stepCount + 1] = splinePoints[_stepCount];

		renderer.renderCatmullRomSpline(
			splinePoints.data(),
			_stepCount + 2,
			Theme::accent1,
			10
		);

		for (uint16_t i = 0; i < _stepCount; ++i) {
			renderer.renderFilledCircle(
				splinePoints[i + 2],
				2,
				Theme::fg1
			);
		}
	}

	void PIDChart::updateValueFactorFromPointerEvent(const int32_t pointerY) {
		const auto bounds = getBounds();
		_valueFactor = 1.f - std::clamp<float>(pointerY - bounds.getY(), 0, bounds.getHeight()) / bounds.getHeight();

		invalidate();
	}

	const PIDCoefficients& PIDChart::getCoefficients() const {
		return _coefficients;
	}

	void PIDChart::setCoefficients(const PIDCoefficients& coefficients) {
		_coefficients = coefficients;

		invalidate();
	}

	void PIDChart::setStepCount(const uint16_t value) {
		_stepCount = value;

		invalidate();
	}

	void PIDChart::setDeltaTime(const float deltaTime) {
		_deltaTime = deltaTime;
	}

	float PIDChart::getValueMax() const {
		return _valueMax;
	}

	void PIDChart::setValueMax(const float value) {
		_valueMax = value;
	}
}
