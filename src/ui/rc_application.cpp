#include <cstdint>
#include "rc_application.h"
#include "resources/fonts.h"
#include "ui/navigation/tab_bar.h"

using namespace yoba;

namespace ui {
	RCApplication &RCApplication::getInstance() {
		static RCApplication instance;

		return instance;
	}

	RCApplication::RCApplication() : Application(
		Size(TFT_HEIGHT, TFT_WIDTH),
		25,

		21,
		22,
		12,
		14
	) {

	}

	void RCApplication::begin() {
		Application::begin();

		// Screen
		getScreen().setDefaultFont(resources::fonts::unscii8);

		// Workspace
		getWorkspace().addChild(new TabBar());
//		getWorkspace().addChild(new Rectangle(Color::water));

//		// Piano
//		_piano.begin();
	}

	void RCApplication::onTick() {
//		_piano.tick();

		Application::onTick();
	}

	void RCApplication::onRender() {
//		_piano.renderStrip();

		Application::onRender();
	}
}