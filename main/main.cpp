#include "rc.h"

extern "C" void app_main(void) {
	pizdanc::RC::getInstance().run();
}