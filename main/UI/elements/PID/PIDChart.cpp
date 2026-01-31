#include <utility>

#include <PIDController.h>

#include "UI/elements/PID/PIDChart.h"
#include "UI/theme.h"

namespace pizda {
	PIDChart::PIDChart() {
		setBackgroundColor(&Theme::bg2);
	}

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
			getBackgroundColor()
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

		std::array<Point, 100> splinePoints {};
		splinePoints[0] = bounds.getBottomLeft();
		splinePoints[1] = splinePoints[0];

		uint16_t localX = 0;
		const auto stepPixels = bounds.getWidth() / (_stepsQuantity - 1);

		PIDController PIDController {};

		for (uint16_t i = 0; i < _stepsQuantity; ++i) {
			value = PIDController.tick(
				value,
				_setpoint,

				_coefficients.p,
				_coefficients.i,
				_coefficients.d,

				_deltaTime,

				0.f,
				100.f
			);

			localX += stepPixels;

			splinePoints[i + 2] = {
				bounds.getX() + localX,
				bounds.getY2() - static_cast<int32_t>(value / 100.f * bounds.getHeight())
			};
		}

		splinePoints[_stepsQuantity + 1] = splinePoints[_stepsQuantity];

		renderer->renderCatmullRomSpline(
			splinePoints.data(),
			_stepsQuantity + 2,
			&Theme::accent1,
			10
		);

		for (uint16_t i = 0; i < _stepsQuantity; ++i) {
			renderer->renderFilledCircle(
				splinePoints[i + 2],
				2,
				&Theme::fg1
			);
		}
	}

	void PIDChart::updateSetpointFromPointerEvent(const int32_t pointerY) {
		const auto bounds = getBounds();
		_setpoint = 100 - std::clamp<int32_t>(pointerY - bounds.getY(), 0, bounds.getHeight()) * 100 / bounds.getHeight();

		invalidate();
	}

	const PIDCoefficients& PIDChart::getCoefficients() const {
		return _coefficients;
	}

	void PIDChart::setCoefficients(const PIDCoefficients& coefficients) {
		_coefficients = coefficients;

		invalidate();
	}

	void PIDChart::setStepsQuantity(const uint16_t stepsQuantity) {
		_stepsQuantity = stepsQuantity;

		invalidate();
	}

	void PIDChart::setDeltaTime(const float deltaTime) {
		_deltaTime = deltaTime;
	}

	PIDChartEditor::PIDChartEditor() {
		*this += &_chart;

		// Buttons
		_buttonsRow.setOrientation(Orientation::horizontal);
		_buttonsRow.setSpacing(1);
		_buttonsRow.setHeight(19);
		setAutoSize(&_buttonsRow);
		*this += &_buttonsRow;

		// Frequency
		addButton(_frequencyButton);

		_frequencyButton.setOnClick([this] {
			uint8_t value = static_cast<uint8_t>(_frequency) + 1;

			if (value > static_cast<uint8_t>(PIDChartEditorFrequency::max))
				value = 0;

			_frequency = static_cast<PIDChartEditorFrequency>(value);

			updateFromFrequency();
		});

		// Steps
		addButton(_stepsButton);

		_stepsButton.setOnClick([this] {
			uint8_t value = static_cast<uint8_t>(_steps) + 1;

			if (value > static_cast<uint8_t>(PIDChartEditorSteps::max))
				value = 0;

			_steps = static_cast<PIDChartEditorSteps>(value);

			updateFromSteps();
		});

		// Initialization
		updateFromFrequency();
		updateFromSteps();
	}

	void PIDChartEditor::addButton(Button& button) {
		Theme::applySecondary(&button);
		button.setCornerRadius(0);
		button.setHeight(Size::computed);
		_buttonsRow += &button;
	}

	void PIDChartEditor::updateFromFrequency() {
		uint8_t frequency;
		std::wstring text;

		switch (_frequency) {
			case PIDChartEditorFrequency::hz1:
				frequency = 1;
				text = L"1 Hz";
				break;
			case PIDChartEditorFrequency::hz20:
				frequency = 20;
				text = L"20 Hz";
				break;
			case PIDChartEditorFrequency::hz30:
				frequency = 30;
				text = L"30 Hz";
				break;
			default:
				frequency = 100;
				text = L"100 Hz";
				break;
		}

		_chart.setDeltaTime(1.f / frequency);
		_frequencyButton.setText(text);
	}

	void PIDChartEditor::updateFromSteps() {
		uint8_t steps;
		std::wstring text;

		switch (_steps) {
			case PIDChartEditorSteps::steps5:
				steps = 5;
				text = L"5 it";
				break;

			case PIDChartEditorSteps::steps10:
				steps = 10;
				text = L"10 it";
				break;

			default:
				steps = 20;
				text = L"20 it";
				break;
		}

		_chart.setStepsQuantity(steps);
		_stepsButton.setText(text);
	}
}
