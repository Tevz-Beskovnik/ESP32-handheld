#pragma once

#include "esp_wifi.h"
#include <WiFi_driver.hpp>
#include <Application.hpp>
#include <memory.h>

class Deauther : public Application
{
};

// void setup_deauther_wifi_ap(uint8_t channel, uint8_t ssid[31], uint8_t password[63]);