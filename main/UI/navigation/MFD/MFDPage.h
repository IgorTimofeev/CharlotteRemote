#pragma once

#include <memory>

#include "rc.h"
#include "UI/navigation/page.h"

#include "PFD/PFD.h"
#include "ND/ND.h"
#include "toolbar/main/mainToolbar.h"
#include "toolbar/autopilot/autopilotToolbar.h"
#include "toolbar/baro/baroToolbar.h"
#include "toolbar/trim/trimToolbar.h"
#include "toolbar/lights/lightsToolbar.h"

namespace pizda {
	using namespace YOBA;

	class MFDPageSplitter : public Control, public ActiveElement {
		public:
			MFDPageSplitter();

		protected:
			void onEvent(Event* event) override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			int32_t _pointerY = -1;
			bool _draggedDohuya = false;
	};

	class PFDAndSplitterLayout : public Layout {
		public:
			PFDAndSplitterLayout();

			PFD _PFD {};
			MFDPageSplitter _splitter {};
	};

	class MFDLayout : public RelativeStackLayout {
		public:
			void deleteShit();
			void createShit();

			std::unique_ptr<PFDAndSplitterLayout> _PFDAndSplitter {};
			std::unique_ptr<ND> _ND {};
	};

	class MFDPage : public Page {
		public:
			explicit MFDPage();

			~MFDPage() override;

			static void fromSettings();

		private:
			static MFDPage* _instance;

			RelativeStackLayout _rows {};

			MFDLayout _MFDLayout {};

			MainToolbar _mainToolbar {};
			std::unique_ptr<AutopilotToolbar> _autopilotToolbar {};
			std::unique_ptr<BaroToolbar> _baroToolbar {};
			std::unique_ptr<TrimToolbar> _trimToolbar {};
			std::unique_ptr<LightsToolbar> _lightsToolbar {};

			void fromSettingsInstance();
	};
}