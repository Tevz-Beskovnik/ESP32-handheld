#include<WiFi_driver.hpp>

WiFi_driver::WiFi_driver(connection_info_t* config)
    :ssid(config->ssid), pass(config->pass), wifi_mode(config->wifi_mode), s_config(NULL)
{
    WiFi.mode(wifi_mode);

    switch(wifi_mode)
    {
        case WIFI_AP:
            WiFi.softAP(ssid, pass);
        case WIFI_STA:
            WiFi.begin(ssid, pass);
            while(WiFi.status() != WL_CONNECTED) connectingAction();
            local_ip = WiFi.localIP();
        case WIFI_AP_STA:
            WiFi.begin(ssid, pass);
            while(WiFi.status() != WL_CONNECTED) connectingAction();
            local_ip = WiFi.localIP();
        default:
            #ifdef DEBUG
            Serial.println("WiFi mode invalid!");
            #endif
            delay(1000);
            ESP.restart();
    }
}

WiFi_driver::WiFi_driver(const char* ssid, const char* pass, wifi_mode_t wifi_mode)
    :ssid(ssid), pass(pass), wifi_mode(wifi_mode), s_config(NULL)
{
    WiFi.mode(wifi_mode);

    switch(wifi_mode)
    {
        case WIFI_AP:
            WiFi.softAP(ssid, pass);
        case WIFI_STA:
            WiFi.begin(ssid, pass);
            while(WiFi.status() != WL_CONNECTED) connectingAction();
            local_ip = WiFi.localIP();
        case WIFI_AP_STA:
            WiFi.begin(ssid, pass);
            while(WiFi.status() != WL_CONNECTED) connectingAction();
            local_ip = WiFi.localIP();
        default:
            #ifdef DEBUG
            Serial.println("WiFi mode invalid!");
            #endif
            delay(1000);
            ESP.restart();
    }
}

WiFi_driver::~WiFi_driver()
{
    WiFi.disconnect();
}

void WiFi_driver::config(connection_settings_t* settings)
{
    s_config = settings;
    IPAddress dns, dns2;
    if(s_config->prim_DNS_c) dns = s_config->prim_DNS_c; else dns = IPAddress(8, 8, 8, 8);
    if(s_config->sec_DNS_c) dns2 = s_config->sec_DNS_c; else dns2 = IPAddress(0, 0, 0, 0);
    if(!WiFi.config(s_config->local_ip, s_config->gateway, s_config->subnet, dns, dns2))
    {
        #ifdef DEBUG
        Serial.println("Cannot configure IP settings!");
        #endif
        delay(1000);
        ESP.restart();
    }
}

void WiFi_driver::changeCredenctials(connection_info_t* conn)
{
    ssid = conn->ssid;
    pass = conn->pass;
    wifi_mode = conn->wifi_mode;

    WiFi.disconnect();
    WiFi.mode(wifi_mode);

    switch(wifi_mode)
    {
        case WIFI_AP:
            WiFi.softAP(ssid, pass);
        case WIFI_STA:
            WiFi.begin(ssid, pass);
            while(WiFi.status() != WL_CONNECTED) connectingAction();
            local_ip = WiFi.localIP();
        case WIFI_AP_STA:
            WiFi.begin(ssid, pass);
            while(WiFi.status() != WL_CONNECTED) connectingAction();
            local_ip = WiFi.localIP();
        default:
            #ifdef DEBUG
            Serial.println("WiFi mode invalid!");
            #endif
            delay(1000);
            ESP.restart();
    }
}

void WiFi_driver::reconnect()
{
    WiFi.reconnect();
}

connection_strength_t WiFi_driver::strength()
{
    int32_t strength = WiFi.RSSI();
    if(strength == 0)
        return NO_CONNECTION;
    else if(strength <= -50 && strength > -60)
        return CONNECTION_EXCELENT;
    else if(strength <= -60 && strength > -70)
        return CONNECTION_VERY_GOOD;
    else if(strength <= -70 && strength > -80)
        return CONNECTION_GOOD;
    else if(strength <= -80 && strength > -90)
        return CONNECTION_LOW;
    else if(strength <= -90)
        return CONNECTION_VERY_LOW;
    else
        return NO_CONNECTION;
}

void WiFi_driver::status()
{
    WiFi.status();
}
