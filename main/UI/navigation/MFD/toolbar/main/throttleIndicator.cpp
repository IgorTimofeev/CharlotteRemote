#include "throttleIndicator.h"

#include <format>

#include "UI/theme.h"
#include "rc.h"

namespace pizda {
	ThrottleIndicator::ThrottleIndicator() {
		setSize(Size(38, 5));
	}
	
	void ThrottleIndicator::onRender(Renderer* renderer, const Bounds& bounds) {
		constexpr static uint8_t textMaxSize = 11;
		constexpr static uint8_t textOffset = 4;
		
		constexpr static uint8_t lineOffset = 1;
		
		const uint16_t frameSize = bounds.getWidth() - textMaxSize - textOffset;
		const auto& settings = RC::getInstance().getSettings();

		// Frame
		renderer->renderRectangle(
			Bounds(
				bounds.getX(),
				bounds.getY(),
				frameSize,
				bounds.getHeight()
			),
			&Theme::bg6
		);

		const auto& renderOffsetLine = [&renderer, &bounds](int32_t pos, const Color* color) {
			renderer->renderVerticalLine(
				Point(
					pos,
					bounds.getY() - lineOffset
				),
				bounds.getHeight() + lineOffset * 2,
				color
			);
		};

		// Secondary value
		const auto secondaryValue = settings.autopilot.autoThrottle ? _remoteValue : _aircraftValue;

		renderOffsetLine(
			bounds.getX() + std::max(static_cast<uint16_t>(frameSize * secondaryValue / 0xFF), static_cast<uint16_t>(1)) - 1,
			&Theme::fg5
		);

		// Primary value
		const auto primaryValue = settings.autopilot.autoThrottle ? _aircraftValue : _remoteValue;
		const auto primaryValueSize = std::max(static_cast<uint16_t>(frameSize * primaryValue / 0xFF), static_cast<uint16_t>(1));
		const auto primaryValueColor = settings.autopilot.autoThrottle ? &Theme::purple : &Theme::green;

		if (primaryValueSize > 1) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY(),
					primaryValueSize,
					bounds.getHeight()
				),
				&Theme::fg1
			);
		}

		renderOffsetLine(bounds.getX() + primaryValueSize - 1, primaryValueColor);

		// Aircraft value text
		const auto text = std::format(L"{:03}", static_cast<int32_t>(primaryValue * 100 / 0xFF));

		renderer->renderString(
			Point(
				bounds.getX() + frameSize + textOffset,
				bounds.getYCenter() - Theme::fontSmall.getHeight() / 2 + 1
			),
			&Theme::fontSmall,
			&Theme::fg2,
			text
		);
	}

	uint8_t ThrottleIndicator::getRemoteValue() const {
		return _remoteValue;
	}

	void ThrottleIndicator::setRemoteValue(uint8_t value) {
		_remoteValue = value;

		invalidate();
	}

	uint8_t ThrottleIndicator::getAircraftValue() const {
		return _aircraftValue;
	}

	void ThrottleIndicator::setAircraftValue(uint8_t value) {
		_aircraftValue = value;

		invalidate();
	}
}