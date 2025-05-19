#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

namespace pizda {
	using namespace YOBA;

	class AirportPlaceholderButton : public Button {
		public:
			explicit AirportPlaceholderButton(bool destination);

		protected:
			void onClick() override;

		private:
			bool _destination;
	};

	class LegPlaceholderButton : public Button {
		public:
			LegPlaceholderButton();

		protected:
			void onClick() override;
	};
}