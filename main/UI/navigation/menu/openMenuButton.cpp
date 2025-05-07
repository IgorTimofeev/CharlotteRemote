#include "openMenuButton.h"

#include "../../theme.h"
#include "../../../rc.h"

namespace pizda {
	void OpenMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().showMenu();
	}
}