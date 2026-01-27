#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"

namespace pizda {
	using namespace YOBA;

	class SpectrumScanningChart : public Control {
		protected:
			void onTick() override;
			void onEvent(Event* event) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			Point _pointerPos { -1, -1 };

			void updatePointerPos(const Point& pointerEventPos);
	};

	class SpectrumScanPage : public ScrollViewPage {
		public:
			SpectrumScanPage();

		protected:
			void onTick() override;
		
		private:
			SpectrumScanningChart chart {};

			RelativeStackLayout frequencyRow {};

			TextField frequencyFromTextField {};
			Titler frequencyFromTitle { L"From (MHz)", &frequencyFromTextField };

			TextField frequencyToTextField {};
			Titler frequencyToTitle { L"To (MHz)", &frequencyToTextField };

			TextField frequencyStepTextField {};
			Titler frequencyStepTitle { L"Step (KHz)", &frequencyStepTextField };

			RelativeStackLayout frequencyShortcutRow {};
			Button frequencyShortcut430_440Button {};
			Button frequencyShortcut470_510Button {};
			Button frequencyShortcut779_787Button {};
			Button frequencyShortcut863_870Button {};
			Button frequencyShortcut902_928Button {};

			Button beginButton {};
	};
}