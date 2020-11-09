//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##           ######   #######  ##    ## ######## ####  ######
// ##          ##    ## ##     ## ###   ## ##        ##  ##    ##
// ##          ##       ##     ## ####  ## ##        ##  ##
// ##          ##       ##     ## ## ## ## ######    ##  ##   ####
// ##          ##       ##     ## ##  #### ##        ##  ##    ##
// ##          ##    ## ##     ## ##   ### ##        ##  ##    ##
// ########     ######   #######  ##    ## ##       ####  ######

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

    fs_load_expander();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// #### ##    ## #### ########    ##      ## #### ######## ####
//  ##  ###   ##  ##     ##       ##  ##  ##  ##  ##        ##
//  ##  ####  ##  ##     ##       ##  ##  ##  ##  ##        ##
//  ##  ## ## ##  ##     ##       ##  ##  ##  ##  ######    ##
//  ##  ##  ####  ##     ##       ##  ##  ##  ##  ##        ##
//  ##  ##   ###  ##     ##       ##  ##  ##  ##  ##        ##
// #### ##    ## ####    ##        ###  ###  #### ##       ####

void init_wifi()
{
    if (!cfg_wifi) // wifi not configured
        return;

    uint8_t i = 0;

    char HOSTNAME[32];
    sprintf(HOSTNAME, "%s_%06x", NAME, ESP.getChipId());

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
// #### ##    ## #### ########     ######   ######  ##
//  ##  ###   ##  ##     ##       ##    ## ##    ## ##
//  ##  ####  ##  ##     ##       ##       ##       ##
//  ##  ## ## ##  ##     ##        ######   ######  ##
//  ##  ##  ####  ##     ##             ##       ## ##
//  ##  ##   ###  ##     ##       ##    ## ##    ## ##
// #### ##    ## ####    ##        ######   ######  ########

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
// #### ##    ## #### ########     ######   #######  ######## ########    ###    ########
//  ##  ###   ##  ##     ##       ##    ## ##     ## ##          ##      ## ##   ##     ##
//  ##  ####  ##  ##     ##       ##       ##     ## ##          ##     ##   ##  ##     ##
//  ##  ## ## ##  ##     ##        ######  ##     ## ######      ##    ##     ## ########
//  ##  ##  ####  ##     ##             ## ##     ## ##          ##    ######### ##
//  ##  ##   ###  ##     ##       ##    ## ##     ## ##          ##    ##     ## ##
// #### ##    ## ####    ##        ######   #######  ##          ##    ##     ## ##

void init_softap()
{
    if (use_wifi) // WiFi is configured, no need for softap
        return;

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(STA_IP, STA_IP, STA_MASK);

    char STA_SSID[32];
    sprintf(STA_SSID, "%s_%06x", NAME, ESP.getChipId());
    WiFi.softAP(STA_SSID, STA_PASS);

    WiFi.scanNetworks(true);

    use_softap = true;
    start_webserver = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// #### ##    ## #### ########    ########  ##    ##  ######
//  ##  ###   ##  ##     ##       ##     ## ###   ## ##    ##
//  ##  ####  ##  ##     ##       ##     ## ####  ## ##
//  ##  ## ## ##  ##     ##       ##     ## ## ## ##  ######
//  ##  ##  ####  ##     ##       ##     ## ##  ####       ##
//  ##  ##   ###  ##     ##       ##     ## ##   ### ##    ##
// #### ##    ## ####    ##       ########  ##    ##  ######

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
// #### ##    ## #### ########    ##      ## ######## ########   ######  ######## ########  ##     ## ######## ########
//  ##  ###   ##  ##     ##       ##  ##  ## ##       ##     ## ##    ## ##       ##     ## ##     ## ##       ##     ##
//  ##  ####  ##  ##     ##       ##  ##  ## ##       ##     ## ##       ##       ##     ## ##     ## ##       ##     ##
//  ##  ## ## ##  ##     ##       ##  ##  ## ######   ########   ######  ######   ########  ##     ## ######   ########
//  ##  ##  ####  ##     ##       ##  ##  ## ##       ##     ##       ## ##       ##   ##    ##   ##  ##       ##   ##
//  ##  ##   ###  ##     ##       ##  ##  ## ##       ##     ## ##    ## ##       ##    ##    ## ##   ##       ##    ##
// #### ##    ## ####    ##        ###  ###  ######## ########   ######  ######## ##     ##    ###    ######## ##     ##

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

    server.on("/light_channels", cfg_page_channels_light);
    server.on("/simple_channels", cfg_page_channels_simple);
    server.on("/shutter_channels", cfg_page_channels_shutter);
    server.on("/timed_channels", cfg_page_channels_timed);

    server.on("/expander", cfg_page_expander);

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
//  #######  ########    ###       ##     ## ########  ########     ###    ######## ########
// ##     ##    ##      ## ##      ##     ## ##     ## ##     ##   ## ##      ##    ##
// ##     ##    ##     ##   ##     ##     ## ##     ## ##     ##  ##   ##     ##    ##
// ##     ##    ##    ##     ##    ##     ## ########  ##     ## ##     ##    ##    ######
// ##     ##    ##    #########    ##     ## ##        ##     ## #########    ##    ##
// ##     ##    ##    ##     ##    ##     ## ##        ##     ## ##     ##    ##    ##
//  #######     ##    ##     ##     #######  ##        ########  ##     ##    ##    ########

void ota_update()
{
    do_ota_update = false;

    char url[256];

    // disconnect MQTT
    if (use_mqtt)
        client.disconnect();

#ifdef USE_SSL
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);
    sprintf(url, "https://%s?t=%s%s_SSL&v=%.2f", update_url, FW_NAME, LANG, VERSION);
#else
    sprintf(url, "http://%s?t=%s%s&v=%.2f", update_url, FW_NAME, LANG, VERSION);
#endif

    // check 4 update
    ESPhttpUpdate.update(net, url);

    // ESP.restart();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
