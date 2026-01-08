#include "baroToolbar.h"

namespace pizda {
	BaroToolbar::BaroToolbar() {
		row += &pressure;
		row += &minimums;
	}
}