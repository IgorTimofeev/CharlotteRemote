#include "axesSettingsPage.h"

#include <EMAFilter.h>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	AxesSettingsPage::AxesSettingsPage() :
		_leverLeftAxisEditor(AxisEditor(&RC::getInstance().getAxes().getLeverLeft())),
		_aileronsAxisEditor(AxisEditor(&RC::getInstance().getAxes().getJoystickHorizontal())),
		_elevatorAxisEditor(AxisEditor(&RC::getInstance().getAxes().getJoystickVertical())),
		_rudderAxisEditor(AxisEditor(&RC::getInstance().getAxes().getRing())),
		_leverRightAxisEditor(AxisEditor(&RC::getInstance().getAxes().getLeverRight()))
	{
		// Page title
		title.setText("Axes");

		// Axes editors
		rows += &_leverLeftAxisEditorTitle;
		rows += &_aileronsAxisEditorTitle;
		rows += &_elevatorAxisEditorTitle;
		rows += &_rudderAxisEditorTitle;
		rows += &_leverRightAxisEditorTitle;

		// Jittering slider
		Theme::apply(&_jitteringThresholdSlider);
		_jitteringThresholdSlider.setFillColor(&Theme::bad1);

		_jitteringThresholdSlider.setMinimumValue(0);
		_jitteringThresholdSlider.setMaximumValue(jitteringThresholdMaxValue);
		_jitteringThresholdSlider.setValue(RC::getInstance().getSettings().axes.jitteringThreshold);

		_jitteringThresholdSlider.setTickCount(10);
		_jitteringThresholdSlider.setBigTickStep(5);
		_jitteringThresholdSlider.setTickLabelBuilder(Slider::int32TickLabelBuilder);

		_jitteringThresholdSlider.setOnValueChanged([this] {
			auto& settings = RC::getInstance().getSettings();

			settings.axes.jitteringThreshold = _jitteringThresholdSlider.getValue();
			settings.axes.writeLater();
		});

		rows += &_jitteringThresholdSliderTitle;

		// Low pass slider
		Theme::apply(&_EMAFilterFactorSlider);
		_EMAFilterFactorSlider.setFillColor(&Theme::good2);
		_EMAFilterFactorSlider.setValue(
			static_cast<float>(RC::getInstance().getSettings().axes.EMAFilterFactor)
			/ static_cast<float>(EMAFilter::maxUint16Factor)
		);

		_EMAFilterFactorSlider.setOnValueChanged([this] {
			auto& settings = RC::getInstance().getSettings();

			settings.axes.EMAFilterFactor = static_cast<uint16_t>(_EMAFilterFactorSlider.getValue() * static_cast<float>(EMAFilter::maxUint16Factor));
			settings.axes.writeLater();
		});

		rows += &_EMAFilterFactorSliderTitle;

		// Initialization
		scrollView.setVerticalPosition(_scrollPosition);
	}

	AxesSettingsPage::~AxesSettingsPage() {
		_scrollPosition = scrollView.getVerticalPosition();
	}

	int32_t AxesSettingsPage::_scrollPosition = 0;

	void AxesSettingsPage::onTick() {
		Layout::onTick();

		invalidate();
	}
}