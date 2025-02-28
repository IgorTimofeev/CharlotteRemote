#include "controlsPage.h"

#include "../../../rc.h"
#include "../../theme.h"

namespace pizda {
	void ControlsPage::setup() {
		auto& rc = RC::getInstance();

		// Page title
		pageTitle.setText(L"Control axis");

		// Axis
		Theme::apply(&_aileronsAxisEditorTitle);
		_aileronsAxisEditor.setAxis(&rc.getJoystickHorizontal());
		rows += &_aileronsAxisEditorTitle;

		Theme::apply(&_elevatorAxisEditorTitle);
		_elevatorAxisEditor.setAxis(&rc.getJoystickVertical());
		rows += &_elevatorAxisEditorTitle;

		Theme::apply(&_rudderAxisEditorTitle);
		_rudderAxisEditor.setAxis(&rc.getRing());
		rows += &_rudderAxisEditorTitle;

		Theme::apply(&_spoilersAxisEditorTitle);
		_spoilersAxisEditor.setAxis(&rc.getLeverLeft());
		rows += &_spoilersAxisEditorTitle;

		Theme::apply(&_flapsAxisEditorTitle);
		_flapsAxisEditor.setAxis(&rc.getLeverRight());
		rows += &_flapsAxisEditorTitle;

		// Controls
		_controls.setHorizontalAlignment(Alignment::start);
		rows += &_controls;
	}
}