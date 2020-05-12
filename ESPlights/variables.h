///////////////////////////////////////////////////////////////////////
// MCP23017 data
///////////////////////////////////////////////////////////////////////
#define MCP_ADDR 0x20    // MCP23017 I2C address
#define GPIOA 0x12       // GPIOA register
bool MCP = false;        // if we found MCP23017 on the bus
uint16_t pins = 0;       // PORTBA pin states
bool pin_states = false; // are pinstates known?
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// ESP supply voltage reporting
///////////////////////////////////////////////////////////////////////
// measure your ESP VCC voltage with a good multimeter
// should be around 3.30V
#define MEASURED 3.306
// enter the ESP reported voltage
// this will fluctuate from ESP to ESP
// i've seen up to 3.39V reported
#define REPORTED 3.44
#define FACTOR 1024 * MEASURED / REPORTED
// use MQTT to send yourself a reminder to change/recharge batteries when voltage drops below 3.0-2.8V
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// hostname will be made up from TIP + "_" + ESPid
///////////////////////////////////////////////////////////////////////
char HOSTNAME[32];
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// periodic reports
///////////////////////////////////////////////////////////////////////
bool heartbeat = true;
#define heartbeat_period 1 * 60 * 1000 // 60 * 1000 = 1 minute
unsigned long last_heartbeat = 0;
///////////////////////////////////////////////////////////////////////