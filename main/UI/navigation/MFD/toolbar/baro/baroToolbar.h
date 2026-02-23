#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/toolbarButton.h"
#include "UI/navigation/MFD/toolbar/baro/referencePressureRotaryControl.h"
#include "UI/navigation/MFD/toolbar/baro/minimumsRotaryControl.h"

#include "resources/images.h"

namespace pizda {
	using namespace YOBA;

	class BaroToolbar : public RowToolbar {
		public:
			BaroToolbar();

			ImageToolbarButton metricUnits { L"Metric", &resources::images::menuIconMfdMetricUnits };
			ReferencePressureRotaryControl pressure {};
			MinimumsRotaryControl minimums {};
	};
}