#include "controlsPage.h"

#include "../../../rc.h"
#include "../../theme.h"

namespace pizda {
	void ControlsPage::setup() {
		auto& rc = RC::getInstance();

		// Page title
		pageTitle.setText(L"Controls");

		// Axis
		_leverLeftEditor.setAxis(&rc.getLeverLeft());
		rows += &_leverLeftEditor;

		_leverRightAxisEditor.setAxis(&rc.getLeverRight());
		rows += &_leverRightAxisEditor;

		_joystickHorizontalAxisEditor.setAxis(&rc.getJoystickHorizontal());
		rows += &_joystickHorizontalAxisEditor;

		_joystickVerticalAxisEditor.setAxis(&rc.getJoystickVertical());
		rows += &_joystickVerticalAxisEditor;

		_ringAxisEditor.setAxis(&rc.getRing());
		rows += &_ringAxisEditor;

		// Controls
		_controls.setHorizontalAlignment(Alignment::start);
		rows += &_controls;
	}
}