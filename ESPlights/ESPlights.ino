ADC_MODE(ADC_VCC);
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <DNSServer.h>
#include <Wire.h>

///////////////////////////////////////////////////////////////////////

#ifdef USE_SSL
#include <WiFiClientSecure.h>
#include <time.h>
#else
#include <WiFiClient.h>
#endif

///////////////////////////////////////////////////////////////////////

#include "variables.h"
#include "webpages.h"

///////////////////////////////////////////////////////////////////////

#ifdef USE_SSL
BearSSL::WiFiClientSecure net;
time_t now;
#else
WiFiClient net;
#endif
MQTTClient client(256);
ESP8266WebServer server(80);
DNSServer dnsServer;

///////////////////////////////////////////////////////////////////////

void initVariant()
{
  WiFi.persistent(false);
}

///////////////////////////////////////////////////////////////////////

void setup()
{
  load_config();    // load configuration from SPIFFS
  init_expander();  // initialize expander
  init_wifi();      // initialize WiFi
  init_softap();    // initialize SoftAP if WiFi not configured
  init_dns();       // initialize DNS for captive portal
  init_mqtt();      // initialize MQTT
  init_webserver(); // initialize Webserver
}

///////////////////////////////////////////////////////////////////////

void loop()
{
  if (use_lights)
    check_timers(); // check timer expiration

  if (use_webserver)
    server.handleClient();

  if (use_dns)
    dnsServer.processNextRequest();

  if (use_mqtt && !client.connected())
    connect_mqtt();

  if (use_mqtt)
    client.loop();

  if (use_mqtt)
    mqtt_loop();

  if (do_ota_update)
    ota_update();

  delay(10);
}

///////////////////////////////////////////////////////////////////////
