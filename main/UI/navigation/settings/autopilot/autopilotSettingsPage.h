#pragma once

#include <functional>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/PID/PIDChart.h"

namespace pizda {
	using namespace YOBA;

	class AutopilotSettingsPage : public ScrollViewPage {
		public:
			AutopilotSettingsPage();

		private:
			PIDChartEditor chartEditor {};

			RelativeStackLayout PIDRow {};

			TextField PTextField {};
			Titler PTitle { L"P", &PTextField };

			TextField ITextField {};
			Titler ITitle { L"I", &ITextField };

			TextField DTextField {};
			Titler DTitle { L"D", &DTextField };

			void updateChart();
	};
}
