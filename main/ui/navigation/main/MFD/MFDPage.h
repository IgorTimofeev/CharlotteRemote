#pragma once

#include <utility>
#include "../../page.h"
#include "../../route.h"

#include "PFD.h"
#include "MFDModeButton.h"

#include "main/mainControls.h"
#include "ND/NDControls.h"
#include "autopilot/autopilotControls.h"
#include "pressure/pressureControls.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MFDPageEbanina {
		public:
			MFDPageEbanina(MFDModeButton* button, Route* route, bool autoSize);

			MFDModeButton* button;
			Route* route;
			bool autoSize;
			Element* element = nullptr;
	};

	class MFDPage : public Page {
		public:
			explicit MFDPage();

		private:
			PFD _PFD {};

			RelativeStackLayout _rows {};

			RelativeStackLayout _buttonsRow {};

			MFDModeButton
				_instrumentsButton = MFDModeButton(L"INS"),
				_NDButton = MFDModeButton(L"N/D"),
				_autopilotButton = MFDModeButton(L"A/P"),
				_pressureButton = MFDModeButton(L"BARO"),
				_menuButton = MFDModeButton(L"...");

			ElementRoute<MainControls> _instrumentsRoute {};
			ElementRoute<NDControls> _NDRoute {};
			ElementRoute<AutopilotControls> _autopilotRoute {};
			ElementRoute<PressureControls> _pressureRoute {};

			MFDPageEbanina _ebaninas[4] {
				{ &_instrumentsButton, &_instrumentsRoute, true },
				{ &_autopilotButton, &_autopilotRoute, true },
				{ &_pressureButton, &_pressureRoute, true },
				{ &_NDButton, &_NDRoute, false },
			};

			void toggleEbanina(MFDPageEbanina* ebanina);
	};
}