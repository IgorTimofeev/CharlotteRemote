#include "controlsPage.h"

#include "../../../rc.h"
#include "../../theme.h"

namespace pizda {
	void ControlsPage::setup() {
		auto& rc = RC::getInstance();

		// Page title
		pageTitle.setText(L"Axis calibration");

		// Axis
		Theme::apply(&_aileronsAxisEditorTitle);
		_aileronsAxisEditor.setup(&rc.getJoystickHorizontal());
		rows += &_aileronsAxisEditorTitle;

		Theme::apply(&_elevatorAxisEditorTitle);
		_elevatorAxisEditor.setup(&rc.getJoystickVertical());
		rows += &_elevatorAxisEditorTitle;

		Theme::apply(&_rudderAxisEditorTitle);
		_rudderAxisEditor.setup(&rc.getRing());
		rows += &_rudderAxisEditorTitle;

		Theme::apply(&_spoilersAxisEditorTitle);
		_spoilersAxisEditor.setup(&rc.getLeverLeft());
		rows += &_spoilersAxisEditorTitle;

		Theme::apply(&_flapsAxisEditorTitle);
		_flapsAxisEditor.setup(&rc.getLeverRight());
		rows += &_flapsAxisEditorTitle;

		// Controls
		_controls.setHorizontalAlignment(Alignment::start);
		rows += &_controls;
	}
}