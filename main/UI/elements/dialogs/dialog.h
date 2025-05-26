#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>

namespace pizda {
	using namespace YOBA;

	class DialogSlideLayoutBackground : public DitheredRectangle {
		public:
			DialogSlideLayoutBackground();

		protected:
			void onTouchUp(TouchUpEvent* event) override;
	};

	class Dialog : public RelativeStackLayout, public ModalElement {
		public:
			Dialog();

			DialogSlideLayoutBackground ditheredBackground {};

			Rectangle slideLayoutBackground {};
			Layout slideLayout {};
	};

	class ScrollViewDialog : public Dialog {
		public:
			ScrollViewDialog();

			ScrollView scrollView {};
			StackLayout rows {};
			Text title {};
	};
}