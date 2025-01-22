#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <esp_timer.h>

#include "rc.h"

extern "C" void app_main(void) {
	pizdanc::RC::getInstance().run();
}