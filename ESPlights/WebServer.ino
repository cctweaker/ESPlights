//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ##    ###    #### ##    ##    ########     ###     ######   ########
// ###   ###   ## ##    ##  ###   ##    ##     ##   ## ##   ##    ##  ##
// #### ####  ##   ##   ##  ####  ##    ##     ##  ##   ##  ##        ##
// ## ### ## ##     ##  ##  ## ## ##    ########  ##     ## ##   #### ######
// ##     ## #########  ##  ##  ####    ##        ######### ##    ##  ##
// ##     ## ##     ##  ##  ##   ###    ##        ##     ## ##    ##  ##
// ##     ## ##     ## #### ##    ##    ##        ##     ##  ######   ########

void page_main()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);
    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);

    sprintf_P(buffer, menu_entry, "/device", menu_entry_device, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/wifi", menu_entry_wifi, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/mqtt", menu_entry_mqtt, "");
    server.sendContent(buffer);
    // sprintf_P(buffer, menu_entry, "/channels", menu_entry_channels, "");
    // server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/light_channels", menu_entry_light_channels, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/simple_channels", menu_entry_simple_channels, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/shutter_channels", menu_entry_shutter_channels, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/timed_channels", menu_entry_timed_channels, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/expander", menu_entry_expander, "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/update", menu_entry_update, menu_entry_check_update);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/format", menu_entry_erase, menu_entry_check_erase);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/restart", menu_entry_restart, menu_entry_check_restart);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/sysinfo", menu_entry_sysinfo, "");
    server.sendContent(buffer);

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ######## ##     ## ####  ######  ########
// ##     ## ##       ##     ##  ##  ##    ## ##
// ##     ## ##       ##     ##  ##  ##       ##
// ##     ## ######   ##     ##  ##  ##       ######
// ##     ## ##        ##   ##   ##  ##       ##
// ##     ## ##         ## ##    ##  ##    ## ##
// ########  ########    ###    ####  ######  ########

void cfg_page_device()
{
    if (server.hasArg("LOC"))
        strlcpy(LOC, server.arg("LOC").c_str(), sizeof(LOC));

    if (server.hasArg("TIP"))
        strlcpy(TIP, server.arg("TIP").c_str(), sizeof(TIP));

    if (server.hasArg("NAME"))
        strlcpy(NAME, server.arg("NAME").c_str(), sizeof(NAME));

    if (server.hasArg("XTRA"))
        strlcpy(XTRA, server.arg("XTRA").c_str(), sizeof(XTRA));

    if (server.hasArg("update_url"))
        strlcpy(update_url, server.arg("update_url").c_str(), sizeof(update_url));

    if (server.hasArg("heartbeat"))
        heartbeat = server.arg("heartbeat").toInt();

    if (server.hasArg("heartbeat_minutes"))
        heartbeat_minutes = server.arg("heartbeat_minutes").toInt();

    if (server.hasArg("start_webserver"))
        start_webserver = server.arg("start_webserver").toInt();

    if (server.hasArg("start_esp_now_bridge"))
        start_esp_now_bridge = server.arg("start_esp_now_bridge").toInt();

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);
    sprintf_P(buffer, form_text_field_string_min, txt_loc, "LOC", LOC);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, txt_tip, "TIP", TIP);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, txt_name, "NAME", NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, txt_xtra, "XTRA", XTRA);
    server.sendContent(buffer);
    sprintf_P(buffer, txt_mqtt_main_topic, LOC, TIP, NAME, XTRA);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_update_url, "update_url", update_url);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, txt_heartbeat, "heartbeat", heartbeat ? "selected" : "", heartbeat ? "" : "selected");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_int_min, txt_heartbeat_minutes, "heartbeat_minutes", heartbeat_minutes);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, txt_start_webserver, "start_webserver", start_webserver ? "selected" : "", start_webserver ? "" : "selected");
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, txt_start_espnow, "start_esp_now_bridge", start_esp_now_bridge ? "selected" : "", start_esp_now_bridge ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_device());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##      ## #### ######## ####
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ######    ##
// ##  ##  ##  ##  ##        ##
// ##  ##  ##  ##  ##        ##
//  ###  ###  #### ##       ####

void cfg_page_wifi()
{
    if (server.hasArg("scan"))
        WiFi.scanNetworks();

    if (server.hasArg("SSIDa"))
        strlcpy(SSIDa, server.arg("SSIDa").c_str(), sizeof(SSIDa));

    if (server.hasArg("PASSa"))
        strlcpy(PASSa, server.arg("PASSa").c_str(), sizeof(PASSa));

    if (server.hasArg("SSIDb"))
        strlcpy(SSIDb, server.arg("SSIDb").c_str(), sizeof(SSIDb));

    if (server.hasArg("PASSb"))
        strlcpy(PASSb, server.arg("PASSb").c_str(), sizeof(PASSb));

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    int i = 0;
    int n = WiFi.scanComplete();

    if (n >= 0)
    {
        server.sendContent_P(form_start);
        for (i = 0; i < n; i++)
        {
            server.sendContent(F("<input type='submit' name='SSIDa' value='"));
            server.sendContent(String() + WiFi.SSID(i) + "'> - ch." + WiFi.channel(i) + " (" + WiFi.RSSI(i) + " dBm)<br>");
        }
        server.sendContent_P(form_end);
        server.sendContent_P(html_hr);
    }

    server.sendContent_P(menu_entry_scan_wifi);
    server.sendContent_P(html_hr);

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "SSID 1", "SSIDa", SSIDa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASS 1", "PASSa", PASSa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "SSID 2", "SSIDb", SSIDb);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASS 2", "PASSb", PASSb);
    server.sendContent(buffer);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_wifi());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ##  #######  ######## ########
// ###   ### ##     ##    ##       ##
// #### #### ##     ##    ##       ##
// ## ### ## ##     ##    ##       ##
// ##     ## ##  ## ##    ##       ##
// ##     ## ##    ##     ##       ##
// ##     ##  ##### ##    ##       ##

void cfg_page_mqtt()
{
    if (server.hasArg("MQTT_HOST"))
        strlcpy(MQTT_HOST, server.arg("MQTT_HOST").c_str(), sizeof(MQTT_HOST));

    if (server.hasArg("MQTT_PORT"))
        MQTT_PORT = server.arg("MQTT_PORT").toInt();

    if (server.hasArg("MQTT_USER"))
        strlcpy(MQTT_USER, server.arg("MQTT_USER").c_str(), sizeof(MQTT_USER));

    if (server.hasArg("MQTT_PASS"))
        strlcpy(MQTT_PASS, server.arg("MQTT_PASS").c_str(), sizeof(MQTT_PASS));

    if (server.hasArg("WILL"))
        strlcpy(WILL, server.arg("WILL").c_str(), sizeof(WILL));

    if (server.hasArg("PUB"))
        strlcpy(PUB, server.arg("PUB").c_str(), sizeof(PUB));

    if (server.hasArg("SUB"))
        strlcpy(SUB, server.arg("SUB").c_str(), sizeof(SUB));

    if (server.hasArg("STAT"))
        strlcpy(STAT, server.arg("STAT").c_str(), sizeof(STAT));

    if (server.hasArg("start_mqtt"))
        start_mqtt = server.arg("start_mqtt").toInt();

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

#ifdef USE_SSL
    server.sendContent_P(txt_ssl_supported);
#endif

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_host, "MQTT_HOST", MQTT_HOST);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_int, txt_mqtt_port, "MQTT_PORT", MQTT_PORT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_user, "MQTT_USER", MQTT_USER);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_pass, "MQTT_PASS", MQTT_PASS);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, txt_mqtt_will, "WILL", WILL);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_mqtt_pub, "PUB", PUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_mqtt_sub, "SUB", SUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, txt_mqtt_stat, "STAT", STAT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, txt_mqtt_start, "start_mqtt", start_mqtt ? "selected" : "", start_mqtt ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_mqtt());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ##    ###    ##    ## ##    ## ######## ##        ######
// ##    ## ##     ##   ## ##   ###   ## ###   ## ##       ##       ##    ##
// ##       ##     ##  ##   ##  ####  ## ####  ## ##       ##       ##
// ##       ######### ##     ## ## ## ## ## ## ## ######   ##        ######
// ##       ##     ## ######### ##  #### ##  #### ##       ##             ##
// ##    ## ##     ## ##     ## ##   ### ##   ### ##       ##       ##    ##
//  ######  ##     ## ##     ## ##    ## ##    ## ######## ########  ######

void cfg_page_channels()
{
    uint8_t i = 0;

    if (server.hasArg("lightadd") && lights < max_channels)
        lights++;
    if (server.hasArg("lightdel") && lights > 0)
        lights--;

    if (server.hasArg("simpleadd") && simple < max_channels)
        simple++;
    if (server.hasArg("simpledel") && simple > 0)
        simple--;

    if (server.hasArg("shadd") && shutters < max_channels / 2)
        shutters++;
    if (server.hasArg("shdel") && shutters > 0)
        shutters--;

    if (server.hasArg("timedadd") && timed < max_channels)
        timed++;
    if (server.hasArg("timeddel") && timed > 0)
        timed--;

    ///////////

    for (i = 0; i < lights; i++)
    {
        String field = "light" + String(i);
        if (server.hasArg(field))
            light_chn[i] = server.arg(field).toInt();
    }

    for (i = 0; i < simple; i++)
    {
        String field = "simple" + String(i);
        if (server.hasArg(field))
            simple_chn[i] = server.arg(field).toInt();
    }

    for (i = 0; i < shutters; i++)
    {
        String field = "shup" + String(i);
        if (server.hasArg(field))
            shutter_chn_up[i] = server.arg(field).toInt();

        field = "shdw" + String(i);
        if (server.hasArg(field))
            shutter_chn_down[i] = server.arg(field).toInt();

        field = "shtmt" + String(i);
        String units = "shtmtu" + String(i);
        if (server.hasArg(field))
            shutter_timing[i] = server.arg(field).toInt() * server.arg(units).toInt();
    }

    for (i = 0; i < timed; i++)
    {
        String field = "timed" + String(i);
        if (server.hasArg(field))
            timed_chn[i] = server.arg(field).toInt();

        field = "timedtmt" + String(i);
        String units = "timedu" + String(i);
        if (server.hasArg(field))
            timed_timing[i] = server.arg(field).toInt() * server.arg(units).toInt();
    }

    ///////////

    yield();

    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        server.sendContent_P(expander_mcp23017);

    if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        server.sendContent_P(expander_pca9557);

    if (io_expander_address == 0x41)
        server.sendContent_P(expander_pca9536);

    server.sendContent_P(html_hr);

    //////////////////////
    // LIGHTS
    //////////////////////
    server.sendContent_P(light_channels);
    sprintf_P(buffer, form_add_del_field, "light", "light");
    server.sendContent(buffer);
    for (i = 0; i < lights; i++)
    {
        sprintf_P(buffer, form_light_field, i + 1, i, light_chn[i]);
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    //////////////////////
    // SIMPLE
    //////////////////////
    server.sendContent_P(simple_channels);
    sprintf_P(buffer, form_add_del_field, "simple", "simple");
    server.sendContent(buffer);
    for (i = 0; i < simple; i++)
    {
        sprintf_P(buffer, form_simple_field, i + 1, i, simple_chn[i]);
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    //////////////////////
    // SHUTTERS
    //////////////////////
    server.sendContent_P(shutter_channels);
    sprintf_P(buffer, form_add_del_field, "sh", "sh");
    server.sendContent(buffer);
    for (i = 0; i < shutters; i++)
    {
        sprintf_P(buffer, shutter_number, i + 1);
        server.sendContent(buffer);
        bool ms = true;
        bool sec = false;
        bool min = false;
        bool hour = false;
        bool day = false;
        uint16_t temp = shutter_timing[i];

        if (shutter_timing[i] % 86400000 == 0) // days
        {
            temp = shutter_timing[i] / 86400000;
            ms = false;
            day = true;
        }
        else if (shutter_timing[i] % 3600000 == 0) // hours
        {
            temp = shutter_timing[i] / 3600000;
            ms = false;
            hour = true;
        }
        else if (shutter_timing[i] % 60000 == 0) // minutes
        {
            temp = shutter_timing[i] / 60000;
            ms = false;
            min = true;
        }
        else if (shutter_timing[i] % 1000 == 0) // seconds
        {
            temp = shutter_timing[i] / 1000;
            ms = false;
            sec = true;
        }

        sprintf_P(buffer, form_shutter_up_field, i, shutter_chn_up[i]);
        server.sendContent(buffer);
        sprintf_P(buffer, form_shutter_down_field, i, shutter_chn_down[i]);
        server.sendContent(buffer);

        sprintf_P(buffer, form_shutter_tmt_field, i, temp);
        server.sendContent(buffer);
        sprintf_P(buffer, form_timeout_select, "shtmtu", i, ms ? "selected" : "", sec ? "selected" : "", min ? "selected" : "", hour ? "selected" : "", day ? "selected" : "");
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    //////////////////////
    // TIMED
    //////////////////////
    server.sendContent_P(timed_channels);
    sprintf_P(buffer, form_add_del_field, "timed", "timed");
    server.sendContent(buffer);
    for (i = 0; i < timed; i++)
    {
        sprintf_P(buffer, timed_number, i + 1);
        server.sendContent(buffer);

        bool ms = true;
        bool sec = false;
        bool min = false;
        bool hour = false;
        bool day = false;
        uint16_t temp = timed_timing[i];

        if (timed_timing[i] % 86400000 == 0) // days
        {
            temp = timed_timing[i] / 86400000;
            ms = false;
            day = true;
        }
        else if (timed_timing[i] % 3600000 == 0) // hours
        {
            temp = timed_timing[i] / 3600000;
            ms = false;
            hour = true;
        }
        else if (timed_timing[i] % 60000 == 0) // minutes
        {
            temp = timed_timing[i] / 60000;
            ms = false;
            min = true;
        }
        else if (timed_timing[i] % 1000 == 0) // seconds
        {
            temp = timed_timing[i] / 1000;
            ms = false;
            sec = true;
        }

        sprintf_P(buffer, form_timed_field, i, timed_chn[i]);
        server.sendContent(buffer);
        sprintf_P(buffer, form_timed_tmt_field, i, temp);
        server.sendContent(buffer);
        sprintf_P(buffer, form_timeout_select, "timedu", i, ms ? "selected" : "", sec ? "selected" : "", min ? "selected" : "", hour ? "selected" : "", day ? "selected" : "");
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_lights());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##       ####  ######   ##     ## ########     ######  ##     ##    ###    ##    ##
// ##        ##  ##    ##  ##     ##    ##       ##    ## ##     ##   ## ##   ###   ##
// ##        ##  ##        ##     ##    ##       ##       ##     ##  ##   ##  ####  ##
// ##        ##  ##   #### #########    ##       ##       ######### ##     ## ## ## ##
// ##        ##  ##    ##  ##     ##    ##       ##       ##     ## ######### ##  ####
// ##        ##  ##    ##  ##     ##    ##       ##    ## ##     ## ##     ## ##   ###
// ######## ####  ######   ##     ##    ##        ######  ##     ## ##     ## ##    ##

void cfg_page_channels_light()
{
    uint8_t i = 0;

    if (server.hasArg("lightadd") && lights < max_channels)
        lights++;
    if (server.hasArg("lightdel") && lights > 0)
        lights--;

    ///////////

    for (i = 0; i < lights; i++)
    {
        String field = "light" + String(i);
        if (server.hasArg(field))
            light_chn[i] = server.arg(field).toInt();
    }

    ///////////

    yield();

    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        server.sendContent_P(expander_mcp23017);

    if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        server.sendContent_P(expander_pca9557);

    if (io_expander_address == 0x41)
        server.sendContent_P(expander_pca9536);

    server.sendContent_P(html_hr);

    //////////////////////
    // LIGHTS
    //////////////////////
    server.sendContent_P(light_channels);
    sprintf_P(buffer, form_add_del_field, "light", "light");
    server.sendContent(buffer);
    for (i = 0; i < lights; i++)
    {
        sprintf_P(buffer, form_light_field, i + 1, i, light_chn[i]);
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_lights());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  #### ##     ## ########  ##       ########     ######  ##     ##    ###    ##    ##
// ##    ##  ##  ###   ### ##     ## ##       ##          ##    ## ##     ##   ## ##   ###   ##
// ##        ##  #### #### ##     ## ##       ##          ##       ##     ##  ##   ##  ####  ##
//  ######   ##  ## ### ## ########  ##       ######      ##       ######### ##     ## ## ## ##
//       ##  ##  ##     ## ##        ##       ##          ##       ##     ## ######### ##  ####
// ##    ##  ##  ##     ## ##        ##       ##          ##    ## ##     ## ##     ## ##   ###
//  ######  #### ##     ## ##        ######## ########     ######  ##     ## ##     ## ##    ##

void cfg_page_channels_simple()
{
    uint8_t i = 0;

    if (server.hasArg("simpleadd") && simple < max_channels)
        simple++;
    if (server.hasArg("simpledel") && simple > 0)
        simple--;

    ///////////

    for (i = 0; i < simple; i++)
    {
        String field = "simple" + String(i);
        if (server.hasArg(field))
            simple_chn[i] = server.arg(field).toInt();
    }

    ///////////

    yield();

    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        server.sendContent_P(expander_mcp23017);

    if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        server.sendContent_P(expander_pca9557);

    if (io_expander_address == 0x41)
        server.sendContent_P(expander_pca9536);

    server.sendContent_P(html_hr);

    //////////////////////
    // SIMPLE
    //////////////////////
    server.sendContent_P(simple_channels);
    sprintf_P(buffer, form_add_del_field, "simple", "simple");
    server.sendContent(buffer);
    for (i = 0; i < simple; i++)
    {
        sprintf_P(buffer, form_simple_field, i + 1, i, simple_chn[i]);
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_lights());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ## ##     ## ######## ######## ######## ########      ######  ##     ##    ###    ##    ##
// ##    ## ##     ## ##     ##    ##       ##    ##       ##     ##    ##    ## ##     ##   ## ##   ###   ##
// ##       ##     ## ##     ##    ##       ##    ##       ##     ##    ##       ##     ##  ##   ##  ####  ##
//  ######  ######### ##     ##    ##       ##    ######   ########     ##       ######### ##     ## ## ## ##
//       ## ##     ## ##     ##    ##       ##    ##       ##   ##      ##       ##     ## ######### ##  ####
// ##    ## ##     ## ##     ##    ##       ##    ##       ##    ##     ##    ## ##     ## ##     ## ##   ###
//  ######  ##     ##  #######     ##       ##    ######## ##     ##     ######  ##     ## ##     ## ##    ##

void cfg_page_channels_shutter()
{
    uint8_t i = 0;

    if (server.hasArg("shadd") && shutters < max_channels / 2)
        shutters++;
    if (server.hasArg("shdel") && shutters > 0)
        shutters--;

    ///////////

    for (i = 0; i < shutters; i++)
    {
        String field = "shup" + String(i);
        if (server.hasArg(field))
            shutter_chn_up[i] = server.arg(field).toInt();

        field = "shdw" + String(i);
        if (server.hasArg(field))
            shutter_chn_down[i] = server.arg(field).toInt();

        field = "shtmt" + String(i);
        String units = "shtmtu" + String(i);
        if (server.hasArg(field))
            shutter_timing[i] = server.arg(field).toInt() * server.arg(units).toInt();
    }

    ///////////

    yield();

    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        server.sendContent_P(expander_mcp23017);

    if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        server.sendContent_P(expander_pca9557);

    if (io_expander_address == 0x41)
        server.sendContent_P(expander_pca9536);

    server.sendContent_P(html_hr);

    //////////////////////
    // SHUTTERS
    //////////////////////
    server.sendContent_P(shutter_channels);
    sprintf_P(buffer, form_add_del_field, "sh", "sh");
    server.sendContent(buffer);
    for (i = 0; i < shutters; i++)
    {
        sprintf_P(buffer, shutter_number, i + 1);
        server.sendContent(buffer);
        bool ms = true;
        bool sec = false;
        bool min = false;
        bool hour = false;
        bool day = false;
        uint16_t temp = shutter_timing[i];

        if (shutter_timing[i] % 86400000 == 0) // days
        {
            temp = shutter_timing[i] / 86400000;
            ms = false;
            day = true;
        }
        else if (shutter_timing[i] % 3600000 == 0) // hours
        {
            temp = shutter_timing[i] / 3600000;
            ms = false;
            hour = true;
        }
        else if (shutter_timing[i] % 60000 == 0) // minutes
        {
            temp = shutter_timing[i] / 60000;
            ms = false;
            min = true;
        }
        else if (shutter_timing[i] % 1000 == 0) // seconds
        {
            temp = shutter_timing[i] / 1000;
            ms = false;
            sec = true;
        }

        sprintf_P(buffer, form_shutter_up_field, i, shutter_chn_up[i]);
        server.sendContent(buffer);
        sprintf_P(buffer, form_shutter_down_field, i, shutter_chn_down[i]);
        server.sendContent(buffer);

        sprintf_P(buffer, form_shutter_tmt_field, i, temp);
        server.sendContent(buffer);
        sprintf_P(buffer, form_timeout_select, "shtmtu", i, ms ? "selected" : "", sec ? "selected" : "", min ? "selected" : "", hour ? "selected" : "", day ? "selected" : "");
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_lights());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ######## #### ##     ## ######## ########      ######  ##     ##    ###    ##    ##
//    ##     ##  ###   ### ##       ##     ##    ##    ## ##     ##   ## ##   ###   ##
//    ##     ##  #### #### ##       ##     ##    ##       ##     ##  ##   ##  ####  ##
//    ##     ##  ## ### ## ######   ##     ##    ##       ######### ##     ## ## ## ##
//    ##     ##  ##     ## ##       ##     ##    ##       ##     ## ######### ##  ####
//    ##     ##  ##     ## ##       ##     ##    ##    ## ##     ## ##     ## ##   ###
//    ##    #### ##     ## ######## ########      ######  ##     ## ##     ## ##    ##

void cfg_page_channels_timed()
{
    uint8_t i = 0;

    if (server.hasArg("timedadd") && timed < max_channels)
        timed++;
    if (server.hasArg("timeddel") && timed > 0)
        timed--;

    ///////////

    for (i = 0; i < timed; i++)
    {
        String field = "timed" + String(i);
        if (server.hasArg(field))
            timed_chn[i] = server.arg(field).toInt();

        field = "timedtmt" + String(i);
        String units = "timedu" + String(i);
        if (server.hasArg(field))
            timed_timing[i] = server.arg(field).toInt() * server.arg(units).toInt();
    }

    ///////////

    yield();

    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        server.sendContent_P(expander_mcp23017);

    if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        server.sendContent_P(expander_pca9557);

    if (io_expander_address == 0x41)
        server.sendContent_P(expander_pca9536);

    server.sendContent_P(html_hr);

    //////////////////////
    // TIMED
    //////////////////////
    server.sendContent_P(timed_channels);
    sprintf_P(buffer, form_add_del_field, "timed", "timed");
    server.sendContent(buffer);
    for (i = 0; i < timed; i++)
    {
        sprintf_P(buffer, timed_number, i + 1);
        server.sendContent(buffer);

        bool ms = true;
        bool sec = false;
        bool min = false;
        bool hour = false;
        bool day = false;
        uint16_t temp = timed_timing[i];

        if (timed_timing[i] % 86400000 == 0) // days
        {
            temp = timed_timing[i] / 86400000;
            ms = false;
            day = true;
        }
        else if (timed_timing[i] % 3600000 == 0) // hours
        {
            temp = timed_timing[i] / 3600000;
            ms = false;
            hour = true;
        }
        else if (timed_timing[i] % 60000 == 0) // minutes
        {
            temp = timed_timing[i] / 60000;
            ms = false;
            min = true;
        }
        else if (timed_timing[i] % 1000 == 0) // seconds
        {
            temp = timed_timing[i] / 1000;
            ms = false;
            sec = true;
        }

        sprintf_P(buffer, form_timed_field, i, timed_chn[i]);
        server.sendContent(buffer);
        sprintf_P(buffer, form_timed_tmt_field, i, temp);
        server.sendContent(buffer);
        sprintf_P(buffer, form_timeout_select, "timedu", i, ms ? "selected" : "", sec ? "selected" : "", min ? "selected" : "", hour ? "selected" : "", day ? "selected" : "");
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_lights());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ######## ##     ## ########     ###    ##    ## ########  ######## ########
// ##        ##   ##  ##     ##   ## ##   ###   ## ##     ## ##       ##     ##
// ##         ## ##   ##     ##  ##   ##  ####  ## ##     ## ##       ##     ##
// ######      ###    ########  ##     ## ## ## ## ##     ## ######   ########
// ##         ## ##   ##        ######### ##  #### ##     ## ##       ##   ##
// ##        ##   ##  ##        ##     ## ##   ### ##     ## ##       ##    ##
// ######## ##     ## ##        ##     ## ##    ## ########  ######## ##     ##

void cfg_page_expander()
{
    uint8_t i = 0;

    for (i = 0; i < max_channels; i++)
    {
        String field = "exp" + String(i);
        if (server.hasArg(field))
            chn_to_expander[i] = server.arg(field).toInt();
    }

    if (server.hasArg("is_relay"))
        is_relay = server.arg("is_relay").toInt();

    ///////////

    yield();

    ///////////

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(form_start);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        server.sendContent_P(expander_mcp23017);

    if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        server.sendContent_P(expander_pca9557);

    if (io_expander_address == 0x41)
        server.sendContent_P(expander_pca9536);

    server.sendContent_P(html_hr);

    //////////////////////
    // channels
    //////////////////////
    server.sendContent_P(txt_chn_to_expander);
    for (i = 0; i < max_channels; i++)
    {
        sprintf_P(buffer, form_expander_field, i + 1, i, chn_to_expander[i]);
        server.sendContent(buffer);
    }

    server.sendContent_P(html_br);
    sprintf_P(buffer, form_yes_no, txt_invert_outputs, "is_relay", is_relay ? "selected" : "", is_relay ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(html_hr);
    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent_P(save_to_config);
        server.sendContent(fs_save_expander());
    }

    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##    ##  ######  #### ##    ## ########  #######
// ##    ##  ##  ##  ##    ##  ##  ###   ## ##       ##     ##
// ##         ####   ##        ##  ####  ## ##       ##     ##
//  ######     ##     ######   ##  ## ## ## ######   ##     ##
//       ##    ##          ##  ##  ##  #### ##       ##     ##
// ##    ##    ##    ##    ##  ##  ##   ### ##       ##     ##
//  ######     ##     ######  #### ##    ## ##        #######

void cfg_page_sysinfo()
{
    if (!chunked_response(200))
        return;

    char buffer[256] = {0};

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    server.sendContent_P(html_table_i);

    ////////////////////////

    sprintf_P(buffer, txt_fw_name, FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_version, VERSION);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_size, ESP.getSketchSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_free_flash_size, ESP.getFreeSketchSpace() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_md5, ESP.getSketchMD5().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_fw_crc, ESP.checkFlashCRC() ? "OK" : "ERROR!!!");
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_core_version, ESP.getCoreVersion().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_sdk_version, ESP.getSdkVersion());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, txt_reset_reason, ESP.getResetReason().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_free_heap, ESP.getFreeHeap() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_max_free_block, ESP.getMaxFreeBlockSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_heap_fragmentation, ESP.getHeapFragmentation());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, txt_esp_chip_id, ESP.getChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_cpu_freq, ESP.getCpuFreqMHz());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_flash_chip_id, ESP.getFlashChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_flash_freq, ESP.getFlashChipSpeed() / 1000000);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_used_flash_size, ESP.getFlashChipSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, txt_real_flash_size, ESP.getFlashChipRealSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////

    server.sendContent_P(html_table_s);

    server.sendContent_P(back_button);
    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ########  ######  ########    ###    ########  ########
// ##     ## ##       ##    ##    ##      ## ##   ##     ##    ##
// ##     ## ##       ##          ##     ##   ##  ##     ##    ##
// ########  ######    ######     ##    ##     ## ########     ##
// ##   ##   ##             ##    ##    ######### ##   ##      ##
// ##    ##  ##       ##    ##    ##    ##     ## ##    ##     ##
// ##     ## ########  ######     ##    ##     ## ##     ##    ##

void page_restart()
{

    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 10);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_restart);
    server.sendContent_P(page_content_main_menu_button);
    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();

    delay(200);
    ESP.restart();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  #######  ########  ##     ##    ###    ########
// ##       ##     ## ##     ## ###   ###   ## ##      ##
// ##       ##     ## ##     ## #### ####  ##   ##     ##
// ######   ##     ## ########  ## ### ## ##     ##    ##
// ##       ##     ## ##   ##   ##     ## #########    ##
// ##       ##     ## ##    ##  ##     ## ##     ##    ##
// ##        #######  ##     ## ##     ## ##     ##    ##

void page_format()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 15);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_erase_settings);
    server.sendContent_P(page_content_main_menu_button);
    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();

    delay(200);

    fs_format();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ## ##     ## ##    ## ##    ## ######## ########
// ##    ## ##     ## ##     ## ###   ## ##   ##  ##       ##     ##
// ##       ##     ## ##     ## ####  ## ##  ##   ##       ##     ##
// ##       ######### ##     ## ## ## ## #####    ######   ##     ##
// ##       ##     ## ##     ## ##  #### ##  ##   ##       ##     ##
// ##    ## ##     ## ##     ## ##   ### ##   ##  ##       ##     ##
//  ######  ##     ##  #######  ##    ## ##    ## ######## ########

bool chunked_response(uint16_t code)
{
    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if (!server.chunkedResponseModeStart(code, "text/html"))
    {
        server.send(505, F("text/html"), F("HTTP1.1 required"));
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ## ########  ########     ###    ######## ########
// ##     ## ##     ## ##     ##   ## ##      ##    ##
// ##     ## ##     ## ##     ##  ##   ##     ##    ##
// ##     ## ########  ##     ## ##     ##    ##    ######
// ##     ## ##        ##     ## #########    ##    ##
// ##     ## ##        ##     ## ##     ##    ##    ##
//  #######  ##        ########  ##     ##    ##    ########

void page_update()
{
    if (!chunked_response(200))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 40);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_update);
    server.sendContent_P(page_content_main_menu_button);
    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();

    delay(200);
    do_ota_update = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##    ##  #######  ########    ########  #######  ##     ## ##    ## ########
// ###   ## ##     ##    ##       ##       ##     ## ##     ## ###   ## ##     ##
// ####  ## ##     ##    ##       ##       ##     ## ##     ## ####  ## ##     ##
// ## ## ## ##     ##    ##       ######   ##     ## ##     ## ## ## ## ##     ##
// ##  #### ##     ##    ##       ##       ##     ## ##     ## ##  #### ##     ##
// ##   ### ##     ##    ##       ##       ##     ## ##     ## ##   ### ##     ##
// ##    ##  #######     ##       ##        #######   #######  ##    ## ########

void page_not_found()
{
    if (!chunked_response(404))
        return;

    char buffer[256];

    sprintf_P(buffer, page_header_start, FW_NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, page_header_refresh, 10);
    server.sendContent(buffer);
    server.sendContent_P(page_header_end);

    sprintf_P(buffer, page_content_title, NAME);
    server.sendContent(buffer);
    server.sendContent_P(page_content_not_found);
    server.sendContent_P(page_content_main_menu_button);
    sprintf_P(buffer, page_footer, FW_NAME, VERSION);
    server.sendContent(buffer);
    server.chunkedResponseFinalize();
}