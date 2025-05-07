#include "axisSettingsPage.h"

#include "../../../../rc.h"
#include "../../../theme.h"

namespace pizda {
	AxisSettingsPage::AxisSettingsPage() :
		_aileronsAxisEditor(AxisEditor(&RC::getInstance().getJoystickHorizontal())),
		_elevatorAxisEditor(AxisEditor(&RC::getInstance().getJoystickVertical())),
		_rudderAxisEditor(AxisEditor(&RC::getInstance().getRing())),
		_spoilersAxisEditor(AxisEditor(&RC::getInstance().getLeverLeft())),
		_flapsAxisEditor(AxisEditor(&RC::getInstance().getLeverRight()))
	{
		// Page title
		title.setText(L"Axis calibration");

		// Axis
		rows += &_aileronsAxisEditorTitle;
		rows += &_elevatorAxisEditorTitle;
		rows += &_rudderAxisEditorTitle;
		rows += &_spoilersAxisEditorTitle;
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

		rows += &_lowPassFactorSliderTitle;
	}

	void AxisSettingsPage::onTick() {
		Layout::onTick();

		invalidate();
	}
}