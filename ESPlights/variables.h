///////////////////////////////////////////////////////////////////////
// configuration file
///////////////////////////////////////////////////////////////////////
const char wifi_cfg_file[] = "wifi.json";
const char dev_cfg_file[] = "device.json";
const char mqtt_cfg_file[] = "mqtt.json";
const char lights_cfg_file[] = "lights.json";
const char expander_cfg_file[] = "expander.json";
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// IO expander stuff
///////////////////////////////////////////////////////////////////////
uint8_t io_expander_address = 0;
uint8_t max_channels = 0;
// 0x20 - 0x27 for MCP23017 16 channel IO expander
// 0x18 - 0x1F for PCA9557 8 channel IO expander
// 0x41        for PCA9536 4 channel IO expander
uint16_t pins = 0;
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// flags
///////////////////////////////////////////////////////////////////////
bool cfg_wifi = false;
bool cfg_dev = false;
bool cfg_mqtt = false;
bool cfg_lights = false;

bool use_wifi = false;
bool use_ssl = false;
bool use_softap = false;
bool use_dns = false;
bool use_mqtt = false;
bool use_webserver = false;
bool use_lights = false;
bool use_esp_now_bridge = false;

bool start_wifi = false;
bool start_mqtt = false;
bool start_webserver = true;
bool start_esp_now_bridge = false;

bool do_ota_update = false;
bool do_save_lights = false;
bool do_init_lights = false;
bool pin_states_known = false;

bool is_relay = false;
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// DEVICE
///////////////////////////////////////////////////////////////////////
const char FW_NAME[] = "ESPlights";
#ifdef LNG
#if LNG == 1
const char LANG[] = "_RO";
#else
const char LANG[] = "";
#endif
#endif

char update_url[128] = "esp.3dstar.ro/";

char LOC[32] = "home/";
char TIP[32] = "lights/";
char NAME[32] = "ESPlights";
char XTRA[32] = "";

bool heartbeat = false;
uint8_t heartbeat_minutes = 60;
unsigned long last_heartbeat = 0;
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// WIFI
///////////////////////////////////////////////////////////////////////
char SSIDa[32] = "";
char PASSa[65] = "";

char SSIDb[32] = "";
char PASSb[65] = "";

const char STA_PASS[] = "abcd1234";
IPAddress STA_IP(192, 168, 10, 1);
IPAddress STA_MASK(255, 255, 255, 0);
const byte DNS_PORT = 53;
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// MQTT
///////////////////////////////////////////////////////////////////////
char MQTT_HOST[128] = "mqtt.3dstar.ro";
int MQTT_PORT = 1883;
char MQTT_USER[32] = "test";
char MQTT_PASS[64] = "12345678";

char WILL[32] = "will";
char PUB[32] = "";
char SUB[32] = "cmnd";
char STAT[32] = "stat";
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// ESP-Now bridge
///////////////////////////////////////////////////////////////////////
uint8_t enb_accolade = 0;            // number of opened accolades
uint8_t enb_position = 0;            // position in rx buffer
char enb_buffer[256];                // rx buffer for relaying
unsigned int enb_accolade_start = 0; // time last message started
bool enb_during_message = false;     // flag that we are in the middle of a message
#define enb_max_time 2 * 1000        // max 2 seconds to finish message
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// CHANNELS
///////////////////////////////////////////////////////////////////////
// mapping channels numbers on device to MCP23017 IO ports
uint8_t chn_to_expander[16] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}; // 3DStar 16 channel controller
// uint8_t chn_to_expander[16] = {8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15, 0}; // Sainsmart relay board
//

// LIGHTS
uint8_t lights = 4;                                                              // number of light channels
uint16_t light_status = 0;                                                       // holds light channels status, 1 bit per channel
uint8_t light_chn[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}; // main channel number assigned per lights channel number

// SIMPLE
uint8_t simple = 2;                                                               // number of simple channels (no status save, no timeout)
uint8_t simple_chn[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}; // main channel number assigned per simple channel number

// channels activated by lights or simple command
// in case same channel is assigned also as timed, then it won't be affected if already on
uint16_t untimed_status = 0;

// SHUTTERS
uint8_t shutters = 2;                                       // number of shutters (each one uses 2 channels, max possible are 8)
uint8_t shutter_status[8] = {0, 0};                         // status per shutter
uint8_t shutter_chn_up[8] = {1, 3, 5, 7, 9, 11, 13, 15};    // main channel number assigned per up command channel per shutter
uint8_t shutter_chn_down[8] = {2, 4, 6, 8, 10, 12, 14, 16}; // main channel number assigned per down command channel per shutter
uint16_t shutter_timing[8] = {30000, 30000};                // timeout in seconds per shutter channel (to kill power), max 65535 seconds
unsigned long shutter_timeout[8] = {0, 0};                  // holds millis when activated, per shutter

// TIMED
uint8_t timed = 2;                                                                                                                  // number timed channels (max 16, timeout in milliseconds)
uint8_t timed_chn[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};                                                    // main channel number assigned per timed channel number
unsigned long timed_timing[16] = {5000, 10000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000}; // timeout in milliseconds per timed channel
unsigned long timed_timeout[16] = {0, 0};                                                                                           // holds millis value when activated, per timed channel
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// Let's Encrypt CA certificate for SSL
// it will authenticate any valid Let's Encrypt issued cert
///////////////////////////////////////////////////////////////////////
#ifdef USE_SSL
static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow
SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT
GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF
q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8
SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0
Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA
a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj
/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T
AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG
CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv
bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k
c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw
VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC
ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz
MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu
Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF
AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo
uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/
wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu
X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG
PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6
KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==
-----END CERTIFICATE-----
)EOF";
#endif
///////////////////////////////////////////////////////////////////////