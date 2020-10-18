//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void load_config()
{
    if (fs_load_wifi())
        cfg_wifi = true;

    if (fs_load_device())
        cfg_dev = true;

    if (fs_load_mqtt())
        cfg_mqtt = true;

    if (fs_load_lights())
        cfg_lights = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_expander()
{
    Wire.begin(2, 0); // connect SDA to GPIO2 and SCL to GPIO0
    check_expander_present();

    if (use_lights)
    {
        // init MCP23017 IO expander
        if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        {
            // set all to low
            Wire.beginTransmission(io_expander_address);
            Wire.write(0x12); // register address for GPIOA
            Wire.write(0);    // GPIOA (0x12)
            Wire.write(0);    // GPIOB (0x13)
            Wire.write(0);    // OLATA (0x14)
            Wire.write(0);    // OLATB (0x15)
            Wire.endTransmission();

            // set all outputs
            Wire.beginTransmission(io_expander_address);
            Wire.write(0x00); // register address for IODIRA
            Wire.write(0);    // IODIRA (0x00), all outputs
            Wire.write(0);    // IODIRB (0x01), all outputs
            Wire.endTransmission();
        }
        // init PCA9557 IO expander
        else if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        {
            // set all to high
            Wire.beginTransmission(io_expander_address);
            Wire.write(1);    // output register
            Wire.write(0xFF); // all high
            Wire.endTransmission();

            // set all outputs
            Wire.beginTransmission(io_expander_address);
            Wire.write(3); // configuration register
            Wire.write(0); // all outputs
            Wire.endTransmission();
        }
        // init PCA9536 IO expander
        else if (io_expander_address == 0x41)
        {
            // set all to high
            Wire.beginTransmission(io_expander_address);
            Wire.write(1);    // output register
            Wire.write(0xFF); // all high
            Wire.endTransmission();

            // set all outputs
            Wire.beginTransmission(io_expander_address);
            Wire.write(3); // configuration register
            Wire.write(0); // all outputs
            Wire.endTransmission();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void check_expander_present()
{
    uint8_t addr = 0;

    // test for MCP23017 16 channel IO expander
    for (addr = 0x20; addr <= 0x27; addr++)
    {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0)
        {
            io_expander_address = addr;
            max_channels = 16;
            use_lights = true;
            return;
        }
    }

    // test for PCA9557 8 channel IO expander
    for (addr = 0x18; addr <= 0x1F; addr++)
    {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0)
        {
            io_expander_address = addr;
            max_channels = 8;
            use_lights = true;
            return;
        }
    }

    // test for PCA9536 4 channel IO expander
    addr = 0x41;
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0)
    {
        io_expander_address = addr;
        max_channels = 4;
        use_lights = true;
        return;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_wifi()
{
    if (!cfg_wifi) // wifi not configured
        return;

    uint8_t i = 0;

    char HOSTNAME[32];
    sprintf(HOSTNAME, "%s_%x", NAME, ESP.getChipId());

    WiFi.mode(WIFI_STA);
    WiFi.hostname(HOSTNAME);

    WiFi.begin(SSIDa, PASSa); // try main WiFi

    i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 200) // wait 20s for main WiFi
    {
        delay(100);
        i++;
    }

    if (WiFi.status() != WL_CONNECTED) // if main WiFi not reachable try second one
    {
        if (strlen(SSIDb) == 0)
            ESP.restart();

        WiFi.begin(SSIDb, PASSb);

        i = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(100);
            i++;
            if (i > 200)
                ESP.restart();
        }
    }

    use_wifi = true;

#ifdef USE_SSL
    init_ssl();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_SSL
void init_ssl()
{
    configTime(+3 * 3600, 0, "esp.3dstar.ro", "3dstar.ro", "pool.ntp.org");
    now = time(nullptr);
    while (now < 1510592825)
    {
        delay(200);
        now = time(nullptr);
    }
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);

    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);

    use_ssl = true;
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_softap()
{
    if (use_wifi) // WiFi is configured, no need for softap
        return;

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(STA_IP, STA_IP, STA_MASK);

    char STA_SSID[32];
    sprintf(STA_SSID, "%s_%06X", NAME, ESP.getChipId());
    WiFi.softAP(STA_SSID, STA_PASS);

    WiFi.scanNetworks(true);

    use_softap = true;
    start_webserver = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_dns()
{

    if (use_wifi) // WiFi is configured, no need for dns
        return;

    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", STA_IP);

    use_dns = true;
    start_webserver = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_mqtt()
{
    if (!start_mqtt || !use_wifi)
        return;

    char topic[128];
    sprintf(topic, "%s/%s/%s/%s", LOC, TIP, NAME, WILL);

    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.setWill(topic, "0", true, 0);
    client.onMessage(messageReceived);

    connect_mqtt();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void connect_mqtt()
{
    uint8_t i = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        i++;
        if (i > 200)
            ESP.restart();
    }

#ifdef USE_SSL
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);
#endif

    i = 0;
    while (!client.connect(NAME, MQTT_USER, MQTT_PASS))
    {
        delay(100);
        i++;
        if (i > 200)
        {
            use_mqtt = false;
            return;
        }
    }

    char topic[128];

    sprintf(topic, "%s/%s/%s/%s/+", LOC, TIP, NAME, SUB);
    client.subscribe(topic, 0);

    sprintf(topic, "%s/%s/%s/%s", LOC, TIP, NAME, WILL);
    client.publish(topic, "1", true, 0);

    mqtt_heartbeat();

    use_mqtt = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_webserver()
{
    if (!use_softap) // if not in SoftAP config mode check if server needs to start
    {
        if (!start_webserver && start_mqtt && use_mqtt) // Webserver can be OFF if MQTT is ON
            return;
    }

    use_webserver = true;

    server.on("/", page_main);
    server.on("/generate_204", page_main);
    server.on("/fwlink", page_main);

    server.on("/device", cfg_page_device);
    server.on("/wifi", cfg_page_wifi);
    server.on("/mqtt", cfg_page_mqtt);
    server.on("/channels", cfg_page_channels);

    server.on("/update", page_update);
    server.on("/format", page_format);
    server.on("/restart", page_restart);
    server.on("/sysinfo", cfg_page_sysinfo);

    server.onNotFound(page_not_found);
    server.begin();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void ota_update()
{
    do_ota_update = false;

    char url[128];

    // disconnect MQTT
    if (use_mqtt)
        client.disconnect();

#ifdef USE_SSL
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);
    sprintf(url, "https://%s?t=%s&v=%.2f", update_url, FW_NAME, VERSION);
#else
    sprintf(url, "http://%s?t=%s&v=%.2f", update_url, FW_NAME, VERSION);
#endif

    // check 4 update
    ESPhttpUpdate.update(net, url);

    ESP.restart();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
