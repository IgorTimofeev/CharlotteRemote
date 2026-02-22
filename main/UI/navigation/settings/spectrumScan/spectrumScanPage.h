#pragma once

#include <functional>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"
#include "UI/elements/dialogs/scrollViewDialog.h"

namespace pizda {
	using namespace YOBA;

	class SpectrumScanningChart : public Control {
		protected:
			void onTick() override;
			void onEvent(Event* event) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			Vector2F _pointerPos { 0.5f, 0.5f };
			int32_t _pinchLength = -1;

			constexpr static int8_t _RSSIMin = -100;
			int8_t _RSSIMax = -20;

			void updatePointerPos(const Point& pointerEventPos);
	};

	class SpectrumScanPage : public RelativeRowsPage {
		public:
			SpectrumScanPage();

		protected:
			void onTick() override;

		private:
			SpectrumScanningChart chart {};

			RelativeStackLayout frequencyRow {};

			TextField frequencyFromTextField {};
			Titler frequencyFromTitle { L"From (MHz)", frequencyFromTextField };

			TextField frequencyToTextField {};
			Titler frequencyToTitle { L"To (MHz)", frequencyToTextField };

			TextField frequencyStepTextField {};
			Titler frequencyStepTitle { L"Step (KHz)", frequencyStepTextField };

			Button frequencyPresetsButton {};

			Button confirmButton {};

			void updateConfirmButtonText();
	};
}