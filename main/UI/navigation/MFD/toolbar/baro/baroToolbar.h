#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/baro/referencePressureRotaryControl.h"
#include "UI/navigation/MFD/toolbar/baro/minimumsRotaryControl.h"

namespace pizda {
	using namespace YOBA;

	class BaroToolbar : public RowToolbar {
		public:
			explicit BaroToolbar();

			ReferencePressureRotaryControl pressure {};
			MinimumsRotaryControl minimums {};
	};
}