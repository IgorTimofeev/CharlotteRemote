#pragma once

#include <format>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>
#include <types/navigationData.h>
#include <utils/rendering.h>

namespace pizda {
	using namespace YOBA;

	class PlaceholderButton : public Button {
		public:
			explicit PlaceholderButton();
	};

	class AirportPlaceholderButton : public PlaceholderButton {
		public:
			explicit AirportPlaceholderButton(bool destination);

		protected:
			void onClick() override;

		private:
			bool _destination;
	};

	class LegPlaceholderButton : public PlaceholderButton {
		public:
			LegPlaceholderButton();

		protected:
			void onClick() override;
	};
}