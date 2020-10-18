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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);

    char buffer[256];

    sprintf_P(buffer, menu_entry, "/device", "Device", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/wifi", "WiFi", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/mqtt", "MQTT", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/channels", "Channels", "");
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/update", "Update firmware", menu_entry_check_update);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/format", "Erase settings", menu_entry_check_erase);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/restart", "Restart", menu_entry_check_restart);
    server.sendContent(buffer);
    sprintf_P(buffer, menu_entry, "/sysinfo", "System information", "");
    server.sendContent(buffer);

    server.sendContent_P(page_footer);
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

    if (server.hasArg("update_url"))
        strlcpy(update_url, server.arg("update_url").c_str(), sizeof(update_url));

    if (server.hasArg("heartbeat"))
        heartbeat = server.arg("heartbeat").toInt();

    if (server.hasArg("heartbeat_minutes"))
        heartbeat_minutes = server.arg("heartbeat_minutes").toInt();

    if (server.hasArg("start_webserver"))
        start_webserver = server.arg("start_webserver").toInt();

    ///////////
    yield();
    ///////////

    if (!chunked_response(200))
        return;

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string_min, "LOC", "LOC", LOC);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, "TIP", "TIP", TIP);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string_min, "NAME", "NAME", NAME);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "Update URL", "update_url", update_url);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, "Heartbeat", "heartbeat", heartbeat ? "selected" : "", heartbeat ? "" : "selected");
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_int_min, "Heartbeat minutes", "heartbeat_minutes", heartbeat_minutes);
    server.sendContent(buffer);
    sprintf_P(buffer, form_yes_no, "Start webserver", "start_webserver", start_webserver ? "selected" : "", start_webserver ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(fs_save_device());
    }

    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
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

    char buffer[256];

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "SSIDa", "SSIDa", SSIDa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASSa", "PASSa", PASSa);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "SSIDb", "SSIDb", SSIDb);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "PASSb", "PASSb", PASSb);
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(fs_save_wifi());
    }

    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);

    char buffer[256];

#ifdef USE_SSL
    server.sendContent(F("<center>Firmware supports SSL connection!</center><br>"));
#endif

    server.sendContent_P(form_start);

    sprintf_P(buffer, form_text_field_string, "MQTT host/ip", "MQTT_HOST", MQTT_HOST);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_int, "MQTT port", "MQTT_PORT", MQTT_PORT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "MQTT user", "MQTT_USER", MQTT_USER);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "MQTT password", "MQTT_PASS", MQTT_PASS);
    server.sendContent(buffer);

    sprintf_P(buffer, form_text_field_string, "WILL subtopic", "WILL", WILL);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "PUB subtopic", "PUB", PUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "SUB subtopic", "SUB", SUB);
    server.sendContent(buffer);
    sprintf_P(buffer, form_text_field_string, "STAT subtopic", "STAT", STAT);
    server.sendContent(buffer);

    sprintf_P(buffer, form_yes_no, "Start MQTT<br>(needs WiFi enabled)", "start_mqtt", start_mqtt ? "selected" : "", start_mqtt ? "" : "selected");
    server.sendContent(buffer);

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(fs_save_mqtt());
    }

    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);

    char buffer[256];

    server.sendContent_P(form_start);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        server.sendContent(F("Using MCP23017 IO expander, 16 channels available.<br>"));

    if (io_expander_address >= 0x18 && io_expander_address <= 0x1F)
        server.sendContent(F("Using PCA9557 IO expander, 8 channels available.<br>"));

    if (io_expander_address == 0x41)
        server.sendContent(F("Using PCA9536 IO expander, 4 channels available.<br>"));

    server.sendContent_P(html_hr);

    //////////////////////
    // LIGHTS
    //////////////////////
    server.sendContent(F("<b>LIGHT channels</b><br>"));
    sprintf_P(buffer, form_add_del_field, "light", "light");
    server.sendContent(buffer);
    for (i = 0; i < lights; i++)
    {
        sprintf_P(buffer, form_light_field, i, i, light_chn[i]);
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    //////////////////////
    // SIMPLE
    //////////////////////
    server.sendContent(F("<b>SIMPLE channels</b><br>"));
    sprintf_P(buffer, form_add_del_field, "simple", "simple");
    server.sendContent(buffer);
    for (i = 0; i < simple; i++)
    {
        sprintf_P(buffer, form_simple_field, i, i, simple_chn[i]);
        server.sendContent(buffer);
    }

    server.sendContent_P(html_hr);

    //////////////////////
    // SHUTTERS
    //////////////////////
    server.sendContent(F("<b>SHUTTER channels</b><br>"));
    sprintf_P(buffer, form_add_del_field, "sh", "sh");
    server.sendContent(buffer);
    for (i = 0; i < shutters; i++)
    {
        server.sendContent("Shutter " + String(i + 1) + "<br>");
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
    server.sendContent(F("<b>TIMED channels</b><br>"));
    sprintf_P(buffer, form_add_del_field, "timed", "timed");
    server.sendContent(buffer);
    for (i = 0; i < timed; i++)
    {
        server.sendContent("Timed " + String(i + 1) + "<br>");

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

    server.sendContent_P(form_buttons);

    if (server.hasArg("save"))
    {
        server.sendContent(F("<hr>Save to config file: "));
        server.sendContent(fs_save_lights());
    }

    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_end);
    server.sendContent_P(html_table_i);

    ////////////////////////

    sprintf_P(buffer, PSTR("<td>FW name</td><td>%s</td>"), FW_NAME);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW version</td><td>%.2f</td>"), VERSION);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW size</td><td>%.2f KB</td>"), ESP.getSketchSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>free flash size</td><td>%.2f KB</td>"), ESP.getFreeSketchSpace() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW MD5</td><td>%s</td>"), ESP.getSketchMD5().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>FW CRC</td><td>%s</td>"), ESP.checkFlashCRC() ? "OK" : "ERROR!!!");
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Core version</td><td>%s</td>"), ESP.getCoreVersion().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>SDK version</td><td>%s</td>"), ESP.getSdkVersion());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, PSTR("<td>Reset reason</td><td>%s</td>"), ESP.getResetReason().c_str());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Free Heap</td><td>%.2f KB</td>"), ESP.getFreeHeap() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Max Free Block</td><td>%.2f KB</td>"), ESP.getMaxFreeBlockSize() / 1024.0);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Heap Fragmentation</td><td>%d%%</td>"), ESP.getHeapFragmentation());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////
    server.sendContent_P(html_table_tr_hr);
    ////////////////////////

    sprintf_P(buffer, PSTR("<td>ESP Chip ID</td><td>%06X</td>"), ESP.getChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>CPU frequency</td><td>%d MHz</td>"), ESP.getCpuFreqMHz());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash Chip ID</td><td>%06X</td>"), ESP.getFlashChipId());
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash frequency</td><td>%d MHz</td>"), ESP.getFlashChipSpeed() / 1000000);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Used flash size</td><td>%d KB</td>"), ESP.getFlashChipSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    sprintf_P(buffer, PSTR("<td>Flash size</td><td>%d KB</td>"), ESP.getFlashChipRealSize() / 1024);
    server.sendContent(buffer);
    server.sendContent_P(html_table_tr);

    ////////////////////////

    server.sendContent_P(html_table_s);

    server.sendContent(F("<br><br><form action='/'><input type='submit' value='Main menu'></form>"));
    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_restart);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_restart);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
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

    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_update);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
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
    server.sendContent_P(page_header_start);
    server.sendContent_P(page_header_refresh);
    server.sendContent_P(page_header_end);
    server.sendContent_P(page_content_title);
    server.sendContent_P(page_content_not_found);
    server.sendContent_P(page_content_main_menu_button);
    server.sendContent_P(page_footer);
    server.chunkedResponseFinalize();
}