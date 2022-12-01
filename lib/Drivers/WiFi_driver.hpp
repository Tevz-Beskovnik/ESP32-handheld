#include <Wire.h>
#include <WiFi.h>

struct connection_info_t {
    const char* ssid;
    const char* pass;
    wifi_mode_t wifi_mode;
};

struct connection_settings_t {
    IPAddress local_ip;
    IPAddress gateway;
    IPAddress subnet;
    bool prim_DNS_c;
    IPAddress prim_DNS;
    bool sec_DNS_c;
    IPAddress sec_DNS;
};

typedef enum {
    CONNECTION_EXCELENT = -50,
    CONNECTION_VERY_GOOD = -60,
    CONNECTION_GOOD = -70,
    CONNECTION_LOW = -80,
    CONNECTION_VERY_LOW = -90,
    NO_CONNECTION = 0
} connection_strength_t;

class WiFi_driver {
    public:
        WiFi_driver(connection_info_t* config);

        WiFi_driver(const char* ssid, const char* pass, wifi_mode_t wifi_mode);

        ~WiFi_driver();

        void config(connection_settings_t* settings);

        void changeCredenctials(connection_info_t* conn);

        void reconnect();

        connection_strength_t strength();

        void status();

    protected: 
        virtual void connectingAction() { delay(1000); }; // function to implement if you want something to happen when network is being connected

    private:
        //config
        connection_settings_t* s_config;

        const char* ssid;
        const char* pass;
        wifi_mode_t wifi_mode;
        IPAddress local_ip;
};