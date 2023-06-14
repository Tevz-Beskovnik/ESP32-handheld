#pragma once

#include "esp_wifi.h"
#include "esp_event.h"

void configure_wifi(wifi_init_config_t cfg, wifi_mode_t mode = WIFI_MODE_STA);