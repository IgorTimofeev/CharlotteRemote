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

			uint32_t frequencyFrom = 0;
			TextField frequencyFromTextField {};
			Titler frequencyFromTitle { L"From (MHz)", &frequencyFromTextField };

			uint32_t frequencyTo = 0;
			TextField frequencyToTextField {};
			Titler frequencyToTitle { L"To (MHz)", &frequencyToTextField };

			uint32_t frequencyStep = 0;
			TextField frequencyStepTextField {};
			Titler frequencyStepTitle { L"Step (KHz)", &frequencyStepTextField };

			Button startButton {};

			void tryParse();
	};
}