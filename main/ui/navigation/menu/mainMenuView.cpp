#include "mainMenuView.h"

#include "../../theme.h"
#include "../../../settings.h"
#include "../../../rc.h"

namespace pizda {
	MainMenuView::MainMenuView() {
		title.setText(L"Main menu");

		wrapLayout += &MFDButton;
		wrapLayout += &axisButton;
		wrapLayout += &WiFiButton;
		wrapLayout += &developerUITestButton;
	}
}