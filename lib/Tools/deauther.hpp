#pragma once

#include "esp_wifi.h"
#include <WiFi_driver.hpp>
#include <Application.hpp>
#include <Console-IO.hpp>
#include <memory.h>
#include <button.hpp>

#define DEAUTHER_MENU_BUTTENS 2

typedef uint8_t mac_address[6];

class Deauther : public Application
{
public:
    Deauther();

    void setup() override;

    bool loop() override;

    void cleanup() override;

private:
    esp_err_t deauth_packet(const mac_address ap, const mac_address station, const mac_address bssid, uint8_t reason, uint8_t channel);

    esp_err_t beacon_packet(const mac_address mac, const char *ssid, uint8_t channel, bool wpa2);

    esp_err_t probe_packet(const mac_address mac, const char *ssid, uint8_t channel);

    esp_err_t change_channel(const uint8_t channel);

    // send raw packet with the ieee 802.11 spec
    esp_err_t send_raw(const uint8_t *packet, int32_t len, bool en_sys_seq = false);

    wifi_config_t ap_config;

    uint16_t sequence_number;

    uint8_t buffer[200];

    // menu config
    uint8_t selected_button;
};

// void setup_deauther_wifi_ap(uint8_t channel, uint8_t ssid[31], uint8_t password[63]);