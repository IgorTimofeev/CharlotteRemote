#pragma once

#include <utility>
#include "../../page.h"
#include "../../route.h"

#include "PFD.h"
#include "MFDModeButton.h"

#include "main/mainControls.h"
#include "ND/NDControls.h"
#include "autopilot/autopilotControls.h"
#include "radio/radioControls.h"

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
				_radioButton = MFDModeButton(L"COM"),
				_menuButton = MFDModeButton(L"...");

			ElementRoute<MainControls> _mainRoute {};
			ElementRoute<NDControls> _NDRoute {};
			ElementRoute<AutopilotControls> _autopilotRoute {};
			ElementRoute<RadioControls> _radioRoute {};

			std::pair<MFDModeButton*, Route*> _buttonsAndRoutes[4] = {
				{ &_mainButton, &_mainRoute },
				{ &_NDButton, &_NDRoute },
				{ &_autopilotButton, &_autopilotRoute },
				{ &_radioButton, &_radioRoute }
			};

			Element* getSelectedControls();

			void selectControls(const std::pair<MFDModeButton*, Route*>& pair);
	};
}