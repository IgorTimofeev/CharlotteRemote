#pragma once

#include "../../../components/yoba/src/main.h"
#include "../../../components/yoba/src/ui.h"

#include "../theme.h"
#include "../../settings.h"
#include "page.h"
#include "route.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MenuButton : public TextElement, public CheckedElement, public PrimaryColorElement {
		public:
			explicit MenuButton(const std::wstring_view& text);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
	};

	class RouteMenuButton : public MenuButton {
		public:
			RouteMenuButton(const std::wstring_view&, const Route* route);

			const Route* getRoute() const;
			void updateSelection();

		protected:
			void onEvent(Event* event) override;

		private:
			const Route* _route;
	};

	class OptionMenuButton : public MenuButton {
		public:
			explicit OptionMenuButton(const std::wstring_view& text);

		protected:
			void onEvent(Event* event) override;
	};

	class MFDModeMenuButton : public MenuButton {
		public:
			explicit MFDModeMenuButton(const std::wstring_view& text, SettingsInterfaceMFDInstrumentsMode mode);

			void updateSelection();

		protected:
			void onEvent(Event* event) override;

		private:
			SettingsInterfaceMFDInstrumentsMode _mode;

	};
}