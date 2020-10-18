///////////////////////////////////////////////////////////////////////
// configuration file
///////////////////////////////////////////////////////////////////////
const char wifi_cfg_file[] = "wifi.json";
const char dev_cfg_file[] = "device.json";
const char mqtt_cfg_file[] = "mqtt.json";
const char lights_cfg_file[] = "lights.json";
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

bool start_wifi = false;
bool start_mqtt = false;
bool start_webserver = false;

bool do_ota_update = false;
bool do_save_lights = false;
bool do_init_lights = false;
bool pin_states_known = false;
///////////////////////////////////////////////////////////////////////

//

///////////////////////////////////////////////////////////////////////
// DEVICE
///////////////////////////////////////////////////////////////////////
const char FW_NAME[] = "ESPlights";
char update_url[128] = "esp.3dstar.ro/";

char LOC[32] = "home";
char TIP[32] = "lights";
char NAME[32] = "lights_ctrl";

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
// CHANNELS
///////////////////////////////////////////////////////////////////////
// mapping channels numbers on device to MCP23017 IO ports
uint8_t chn_to_mcp[16] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
//

// LIGHTS
uint8_t lights = 4;                   // number of light channels
uint16_t light_status = 0;            // holds light channels status, 1 bit per channel
uint8_t light_chn[16] = {1, 2, 3, 4}; // main channel number assigned per lights channel number

// SIMPLE
uint8_t simple = 2;              // number of simple channels (no status save, no timeout)
uint8_t simple_chn[16] = {5, 6}; // main channel number assigned per simple channel number

// SHUTTERS
uint8_t shutters = 2;                        // number of shutters (each one uses 2 channels, max possible are 8)
uint8_t shutter_status[8] = {0, 0};          // status per shutter
uint8_t shutter_chn_up[8] = {13, 15};        // main channel number assigned per up command channel per shutter
uint8_t shutter_chn_down[8] = {14, 16};      // main channel number assigned per down command channel per shutter
uint16_t shutter_timing[8] = {30000, 30000}; // timeout in seconds per shutter channel (to kill power), max 65535 seconds
unsigned long shutter_timeout[8] = {0, 0};   // holds millis when activated, per shutter

// TIMED
uint8_t timed = 2;                              // number timed channels (max 16, timeout in milliseconds)
uint8_t timed_chn[16] = {11, 12};               // main channel number assigned per timed channel number
unsigned long timed_timing[16] = {5000, 10000}; // timeout in milliseconds per timed channel
unsigned long timed_timeout[16] = {0, 0};       // holds millis value when activated, per tmtmin channel
///////////////////////////////////////////////////////////////////////
