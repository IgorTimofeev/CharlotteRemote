#include "axesSettingsPage.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	AxesSettingsPage::AxesSettingsPage() :
		_aileronsAxisEditor(AxisEditor(&RC::getInstance().getAxes().getJoystickHorizontal())),
		_elevatorAxisEditor(AxisEditor(&RC::getInstance().getAxes().getJoystickVertical())),
		_rudderAxisEditor(AxisEditor(&RC::getInstance().getAxes().getRing())),
		_spoilersAxisEditor(AxisEditor(&RC::getInstance().getAxes().getLeverLeft())),
		_flapsAxisEditor(AxisEditor(&RC::getInstance().getAxes().getLeverRight()))
	{
		// Page title
		title.setText(L"Axis");

		// Axis
		rows += &_aileronsAxisEditorTitle;
		rows += &_elevatorAxisEditorTitle;
		rows += &_rudderAxisEditorTitle;
		rows += &_spoilersAxisEditorTitle;
		rows += &_flapsAxisEditorTitle;

		// Jittering slider
		Theme::apply(&_jitteringCutoffFactorSlider);
		_jitteringCutoffFactorSlider.setFillColor(&Theme::bad1);

		_jitteringCutoffFactorSlider.setValue(static_cast<float>(RC::getInstance().getSettings().axes.jitteringCutoffValue) / _jitteringCutoffMaxValue);

		_jitteringCutoffFactorSlider.valueChanged += [this] {
			auto& settings = RC::getInstance().getSettings();

			settings.axes.jitteringCutoffValue = _jitteringCutoffFactorSlider.getValueFactor() * _jitteringCutoffMaxValue;
			settings.axes.scheduleWrite();
		};

		rows += &_jitteringValueSliderTitle;

		// Low pass slider
		Theme::apply(&_lowPassFactorSlider);
		_lowPassFactorSlider.setFillColor(&Theme::good2);

		_lowPassFactorSlider.setValue(static_cast<float>(RC::getInstance().getSettings().axes.lowPassFactor) / 0xFFFF);

		_lowPassFactorSlider.valueChanged += [this] {
			auto& settings = RC::getInstance().getSettings();

			settings.axes.lowPassFactor = _lowPassFactorSlider.getValueFactor() * 0xFFFF;
			settings.axes.scheduleWrite();
		};

		rows += &_lowPassFactorSliderTitle;
	}

	void AxesSettingsPage::onTick() {
		Layout::onTick();

		invalidate();
	}
}