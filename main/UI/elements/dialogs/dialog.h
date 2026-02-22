#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class DialogSlideLayoutBackground : public DitheredRectangle {
		public:
			DialogSlideLayoutBackground();

		protected:
			void onEvent(Event& event) override;
	};

	class Dialog : public RelativeStackLayout {
		public:
			Dialog();

			DialogSlideLayoutBackground ditheredBackground {};

			Rectangle slideLayoutBackground {};
			Layout slideLayout {};

			void show();
			void hide();
	};
}