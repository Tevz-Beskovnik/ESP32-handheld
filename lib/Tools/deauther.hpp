#pragma once

#include "esp_wifi.h"
#include <WiFi_driver.hpp>
#include <Application.hpp>
#include <memory.h>

class Deauther : public Application
{
public:
    Deauther();

    void setup() override;

    bool loop() override;

    void cleanup() override;

private:
    esp_err_t deauth();

    esp_err_t beacon();

    esp_err_t probe();

    // lole
    esp_err_t send_raw(const uint8_t *packet, int32_t len, bool en_sys_seq);

    esp_err_t switch_channel(const uint8_t channel);

    wifi_config_t ap_config;
};

// void setup_deauther_wifi_ap(uint8_t channel, uint8_t ssid[31], uint8_t password[63]);