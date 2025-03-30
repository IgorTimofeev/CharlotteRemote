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

	class MFDPage : public Page {
		public:
			explicit MFDPage();

		private:
			PFD _PFD {};

			EqualLayout _rows {};

			EqualLayout _buttonsRow {};

			MFDModeButton
				_mainButton = MFDModeButton(L"INS"),
				_NDButton = MFDModeButton(L"N/D"),
				_autopilotButton = MFDModeButton(L"A/P"),
				_pressureButton = MFDModeButton(L"BARO"),
				_menuButton = MFDModeButton(L"...");

			ElementRoute<MainControls> _mainRoute {};
			ElementRoute<NDControls> _NDRoute {};
			ElementRoute<AutopilotControls> _autopilotRoute {};
			ElementRoute<PressureControls> _pressureRoute {};

			std::pair<MFDModeButton*, Route*> _buttonsAndRoutes[4] = {
				{ &_mainButton, &_mainRoute },
				{ &_NDButton, &_NDRoute },
				{ &_autopilotButton, &_autopilotRoute },
				{ &_pressureButton, &_pressureRoute }
			};

			Element* getSelectedControls();

			void selectControls(const std::pair<MFDModeButton*, Route*>& pair);
	};
}