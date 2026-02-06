#include "axesSettingsPage.h"

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
		title.setText(L"Axis");

		// Axes editors
		rows += &_leverLeftAxisEditorTitle;
		rows += &_aileronsAxisEditorTitle;
		rows += &_elevatorAxisEditorTitle;
		rows += &_rudderAxisEditorTitle;
		rows += &_leverRightAxisEditorTitle;

		// Jittering slider
		Theme::apply(&_jitteringCutoffFactorSlider);
		_jitteringCutoffFactorSlider.setFillColor(&Theme::bad1);

		_jitteringCutoffFactorSlider.setMinimumValue(0);
		_jitteringCutoffFactorSlider.setMaximumValue(jitteringCutoffMaxValue);
		_jitteringCutoffFactorSlider.setValue(RC::getInstance().getSettings().axes.jitteringCutoffValue);

		_jitteringCutoffFactorSlider.setTickQuantity(10);
		_jitteringCutoffFactorSlider.setBigTickStep(5);
		_jitteringCutoffFactorSlider.setTickLabelBuilder(Slider::int32TickLabelBuilder);

		_jitteringCutoffFactorSlider.setOnValueChanged([this] {
			auto& settings = RC::getInstance().getSettings();

			settings.axes.jitteringCutoffValue = _jitteringCutoffFactorSlider.getValue();
			settings.axes.scheduleWrite();
		});

		rows += &_jitteringValueSliderTitle;

		// Low pass slider
		Theme::apply(&_lowPassFactorSlider);
		_lowPassFactorSlider.setFillColor(&Theme::good2);

		_lowPassFactorSlider.setValue(static_cast<float>(RC::getInstance().getSettings().axes.lowPassFactor) / 0xFFFF);

		_lowPassFactorSlider.setOnValueChanged([this] {
			auto& settings = RC::getInstance().getSettings();

			settings.axes.lowPassFactor = _lowPassFactorSlider.getValueFactor() * 0xFFFF;
			settings.axes.scheduleWrite();
		});

		rows += &_lowPassFactorSliderTitle;
	}

	void AxesSettingsPage::onTick() {
		Layout::onTick();

		invalidate();
	}
}