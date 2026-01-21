#include "legPlaceholderButton.h"

#include "UI/theme.h"
#include "UI/elements/navigation/waypointButton.h"
#include "types/navigationData.h"

#include "flightPlanItemDialog.h"

namespace pizda {
	LegPlaceholderButton::LegPlaceholderButton() {
		Theme::applyPlaceholder(this);
		setHeight(WaypointButton::height);
		setText(L"Add route");
	}

	void LegPlaceholderButton::onClick() {
		Button::onClick();

		FlightPlanItemDialog::showWaypointSelectionDialogToInsertAt(getText(), 0);
	}
}
