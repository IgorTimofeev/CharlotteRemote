#include "axisPage.h"

#include "../../../rc.h"
#include "../../theme.h"

namespace pizda {
	AxisPage::AxisPage() :
		_aileronsAxisEditor(AxisEditor(&RC::getInstance().getJoystickHorizontal())),
		_elevatorAxisEditor(AxisEditor(&RC::getInstance().getJoystickVertical())),
		_rudderAxisEditor(AxisEditor(&RC::getInstance().getRing())),
		_spoilersAxisEditor(AxisEditor(&RC::getInstance().getLeverLeft())),
		_flapsAxisEditor(AxisEditor(&RC::getInstance().getLeverRight()))
	{
		// Page title
		title.setText(L"Axis calibration");

		// Axis
		Theme::apply(&_aileronsAxisEditorTitle);
		rows += &_aileronsAxisEditorTitle;

		Theme::apply(&_elevatorAxisEditorTitle);
		rows += &_elevatorAxisEditorTitle;

		Theme::apply(&_rudderAxisEditorTitle);
		rows += &_rudderAxisEditorTitle;

		Theme::apply(&_spoilersAxisEditorTitle);
		rows += &_spoilersAxisEditorTitle;

		Theme::apply(&_flapsAxisEditorTitle);
		rows += &_flapsAxisEditorTitle;

		// Jittering slider
		Theme::apply(&_jitteringCutoffFactorSlider);
		_jitteringCutoffFactorSlider.setFillColor(&Theme::bad2);

		_jitteringCutoffFactorSlider.setValue(RC::getInstance().getSettings().axis.jitteringCutoffValue * 0xFFFF / _jitteringCutoffMaxValue);

		_jitteringCutoffFactorSlider.valueChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();

			settings.axis.jitteringCutoffValue = _jitteringCutoffFactorSlider.getValue() * _jitteringCutoffMaxValue / 0xFFFF;
			settings.enqueueWrite();
		};

		Theme::apply(&_jitteringValueSliderTitle);
		rows += &_jitteringValueSliderTitle;

		// Low pass slider
		Theme::apply(&_lowPassFactorSlider);
		_lowPassFactorSlider.setFillColor(&Theme::good2);

		_lowPassFactorSlider.setValue(RC::getInstance().getSettings().axis.lowPassFactor);

		_lowPassFactorSlider.valueChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();

			settings.axis.lowPassFactor = _lowPassFactorSlider.getValue();
			settings.enqueueWrite();
		};

		Theme::apply(&_lowPassFactorSliderTitle);
		rows += &_lowPassFactorSliderTitle;
	}

	void AxisPage::onTick() {
		Layout::onTick();

		invalidate();
	}
}