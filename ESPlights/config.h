///////////////////////////////////////////////////////////////////////
// device type (used in HOSTNAME and MQTT client) combined with ESP id
///////////////////////////////////////////////////////////////////////
char TIP[8] = "lights";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// device location also used as topic prefix
///////////////////////////////////////////////////////////////////////
char LOC[16] = "location";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// primary WIFI
///////////////////////////////////////////////////////////////////////
char SSIDa[32] = "WiFi";
char PASSa[64] = "password";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// secondary WIFI
///////////////////////////////////////////////////////////////////////
char SSIDb[32] = "WiFi";
char PASSb[64] = "password";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// MQTT login
///////////////////////////////////////////////////////////////////////
char MQTT_HOST[64] = "www.mqtt.server";
int MQTT_PORT = 8883;
char MQTT_USER[64] = "mqtt_user";
char MQTT_PASS[64] = "mqtt_password";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// update server, keep final / if path or end in .php if file
///////////////////////////////////////////////////////////////////////
char update_url[128] = "https://esp.3dstar.ro/";
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// pin mapping for channel to MCP pins (3Dstar.ro PCB)
///////////////////////////////////////////////////////////////////////
uint8_t chn_to_mcp[16] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// shutters, doors, gates, simple command
///////////////////////////////////////////////////////////////////////
// number of shutters (max 8)
uint8_t shutters = 2;
// status per shutter
uint8_t shutter_status[8] = {0, 0};
// up channel per shutter
uint8_t shutter_chn_up[8] = {11, 13};
// down channel per shutter
uint8_t shutter_chn_down[8] = {12, 14};
// timeout in seconds per shutter
uint8_t shutter_timing[8] = {30, 30};
// holds millis when activated, per shutter
unsigned long shutter_timeout[8] = {0, 0};

// number of doors (max 16, timeout in seconds)
uint8_t doors = 1;
// channel per door
uint8_t door_chn[16] = {15};
// timeout in seconds per door
uint8_t door_timing[16] = {10};
// holds millis when activated, per door
unsigned long door_timeout[16] = {0};

// number of gates (max 16, timeout in ms)
uint8_t gates = 1;
// channel per gate
uint8_t gate_chn[16] = {16};
// timeout in ms per gate
uint16_t gate_timing[16] = {200};
// holds millis when activated, per gate
unsigned long gate_timeout[16] = {0};

// number of lights channels, max 16, status saved on mqtt server
uint8_t lights = 8;
uint16_t light_status = 0;
uint8_t light_chn[16] = {1, 2, 3, 4, 5, 6, 7, 8};

// number of simple channels, status not saved
uint8_t simple = 2;
uint8_t simple_chn[16] = {9, 10};
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// heartbeat with information by mqtt
///////////////////////////////////////////////////////////////////////
// on - true or false - off
bool heartbeat = true;
// time in minutes between messages
uint16_t heartbeat_minutes = 10;
///////////////////////////////////////////////////////////////////////
