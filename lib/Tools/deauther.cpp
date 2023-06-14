#include <deauther.hpp>

void setup_deauther_wifi_ap(uint8_t channel, uint8_t ssid[31], uint8_t password[63])
{
    wifi_init_config_t wifi_conf = WIFI_INIT_CONFIG_DEFAULT();

    configure_wifi(wifi_conf, WIFI_MODE_AP);

    wifi_config_t ap_config = {
        .ap = {
            .ssid_len = 22,
            .channel = channel,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .ssid_hidden = 0,
            .max_connection = 4,
            .beacon_interval = 60000}};

    MEMCPY(ap_config.ap.ssid, ssid, 31);

    MEMCPY(ap_config.ap.password, password, 63);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
}

Deauther::Deauther()
    : Application("Deauther")
{
    char ssid[10];
    char password[10];

    kb->prompt_string(ssid, 10);
    kb->prompt_string(password, 10);

    ap_config = {
        .ap = {
            .ssid_len = 22,
            .channel = 1,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .ssid_hidden = 0,
            .max_connection = 4,
            .beacon_interval = 60000}};

    MEMCPY(ap_config.ap.ssid, ssid, 10);

    MEMCPY(ap_config.ap.password, password, 10);
}

void Deauther::setup()
{
}

bool Deauther::loop()
{
}

void Deauther::cleanup()
{
}