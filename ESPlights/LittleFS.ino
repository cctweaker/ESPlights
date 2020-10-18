//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ##      ## #### ######## ####
// ##          ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ######    ##
// ##          ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ##        ##
// ########     ###  ###  #### ##       ####

bool fs_load_wifi()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(wifi_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(512);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("SSIDa"))
        strlcpy(SSIDa, doc["SSIDa"], sizeof(SSIDa));
    if (doc.containsKey("PASSa"))
        strlcpy(PASSa, doc["PASSa"], sizeof(PASSa));

    if (doc.containsKey("SSIDb"))
        strlcpy(SSIDb, doc["SSIDb"], sizeof(SSIDb));
    if (doc.containsKey("PASSb"))
        strlcpy(PASSb, doc["PASSb"], sizeof(PASSb));

    if (doc.containsKey("start_wifi"))
        start_wifi = doc["start_wifi"];

    doc.clear();

    LittleFS.end();

    // if ((strlen(SSIDa) && strlen(PASSa)) || (strlen(SSIDb) && strlen(PASSb)))
    return true;

    // return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ########  ######## ##     ## ####  ######  ########
// ##          ##     ## ##       ##     ##  ##  ##    ## ##
// ##          ##     ## ##       ##     ##  ##  ##       ##
// ##          ##     ## ######   ##     ##  ##  ##       ######
// ##          ##     ## ##        ##   ##   ##  ##       ##
// ##          ##     ## ##         ## ##    ##  ##    ## ##
// ########    ########  ########    ###    ####  ######  ########

bool fs_load_device()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(dev_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(512);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("LOC"))
        strlcpy(LOC, doc["LOC"], sizeof(LOC));
    if (doc.containsKey("TIP"))
        strlcpy(TIP, doc["TIP"], sizeof(TIP));
    if (doc.containsKey("NAME"))
        strlcpy(NAME, doc["NAME"], sizeof(NAME));

    if (doc.containsKey("update_url"))
        strlcpy(update_url, doc["update_url"], sizeof(update_url));

    if (doc.containsKey("heartbeat"))
        heartbeat = doc["heartbeat"];
    if (doc.containsKey("heartbeat_minutes"))
        heartbeat_minutes = doc["heartbeat_minutes"];

    if (doc.containsKey("start_webserver"))
        start_webserver = doc["start_webserver"];

    doc.clear();

    LittleFS.end();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ##     ##  #######  ######## ########
// ##          ###   ### ##     ##    ##       ##
// ##          #### #### ##     ##    ##       ##
// ##          ## ### ## ##     ##    ##       ##
// ##          ##     ## ##  ## ##    ##       ##
// ##          ##     ## ##    ##     ##       ##
// ########    ##     ##  ##### ##    ##       ##

bool fs_load_mqtt()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(mqtt_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(512);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    if (doc.containsKey("MQTT_HOST"))
        strlcpy(MQTT_HOST, doc["MQTT_HOST"], sizeof(MQTT_HOST));
    if (doc.containsKey("MQTT_PORT"))
        MQTT_PORT = doc["MQTT_PORT"];
    if (doc.containsKey("MQTT_USER"))
        strlcpy(MQTT_USER, doc["MQTT_USER"], sizeof(MQTT_USER));
    if (doc.containsKey("MQTT_PASS"))
        strlcpy(MQTT_PASS, doc["MQTT_PASS"], sizeof(MQTT_PASS));

    if (doc.containsKey("WILL"))
        strlcpy(WILL, doc["WILL"], sizeof(WILL));
    if (doc.containsKey("PUB"))
        strlcpy(PUB, doc["PUB"], sizeof(PUB));
    if (doc.containsKey("SUB"))
        strlcpy(SUB, doc["SUB"], sizeof(SUB));
    if (doc.containsKey("STAT"))
        strlcpy(STAT, doc["STAT"], sizeof(STAT));

    if (doc.containsKey("start_mqtt"))
        start_mqtt = doc["start_mqtt"];

    doc.clear();

    LittleFS.end();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##          ##       ####  ######   ##     ## ########  ######
// ##          ##        ##  ##    ##  ##     ##    ##    ##    ##
// ##          ##        ##  ##        ##     ##    ##    ##
// ##          ##        ##  ##   #### #########    ##     ######
// ##          ##        ##  ##    ##  ##     ##    ##          ##
// ##          ##        ##  ##    ##  ##     ##    ##    ##    ##
// ########    ######## ####  ######   ##     ##    ##     ######

bool fs_load_lights()
{
    if (!LittleFS.begin())
        return false;

    // open config file for reading
    File configFile = LittleFS.open(lights_cfg_file, "r");
    if (!configFile)
        return false;

    DynamicJsonDocument doc(2048);

    // Read content from config file.
    deserializeJson(doc, configFile);
    configFile.close();
    LittleFS.end();

    uint8_t i = 0;

    if (doc.containsKey("chn_to_mcp"))
        for (i = 0; i < 16; i++)
            chn_to_mcp[i] = doc["chn_to_mcp"][i];

    if (doc.containsKey("lights"))
        lights = doc["lights"];
    if (doc.containsKey("light_chn"))
        for (i = 0; i < lights; i++)
            light_chn[i] = doc["light_chn"][i];

    if (doc.containsKey("simple"))
        simple = doc["simple"];
    if (doc.containsKey("simple_chn"))
        for (i = 0; i < simple; i++)
            simple_chn[i] = doc["simple_chn"][i];

    if (doc.containsKey("shutters"))
        shutters = doc["shutters"];
    if (doc.containsKey("shutter_chn_up"))
        for (i = 0; i < shutters; i++)
            shutter_chn_up[i] = doc["shutter_chn_up"][i];
    if (doc.containsKey("shutter_chn_down"))
        for (i = 0; i < shutters; i++)
            shutter_chn_down[i] = doc["shutter_chn_down"][i];
    if (doc.containsKey("shutter_timing"))
        for (i = 0; i < shutters; i++)
            shutter_timing[i] = doc["shutter_timing"][i];

    if (doc.containsKey("timed"))
        timed = doc["timed"];
    if (doc.containsKey("timed_chn"))
        for (i = 0; i < timed; i++)
            timed_chn[i] = doc["timed_chn"][i];
    if (doc.containsKey("timed_timing"))
        for (i = 0; i < timed; i++)
            timed_timing[i] = doc["timed_timing"][i];

    doc.clear();

    LittleFS.end();

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ##      ## #### ######## ####
// ##    ##    ##  ##  ##  ##  ##        ##
// ##          ##  ##  ##  ##  ##        ##
//  ######     ##  ##  ##  ##  ######    ##
//       ##    ##  ##  ##  ##  ##        ##
// ##    ##    ##  ##  ##  ##  ##        ##
//  ######      ###  ###  #### ##       ####

const char *fs_save_wifi()
{
    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(2048);

    doc["SSIDa"] = SSIDa;
    doc["PASSa"] = PASSa;
    doc["SSIDb"] = SSIDb;
    doc["PASSb"] = PASSb;

    doc["start_wifi"] = start_wifi;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(wifi_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    LittleFS.end();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ########  ######## ##     ## ####  ######  ########
// ##    ##    ##     ## ##       ##     ##  ##  ##    ## ##
// ##          ##     ## ##       ##     ##  ##  ##       ##
//  ######     ##     ## ######   ##     ##  ##  ##       ######
//       ##    ##     ## ##        ##   ##   ##  ##       ##
// ##    ##    ##     ## ##         ## ##    ##  ##    ## ##
//  ######     ########  ########    ###    ####  ######  ########

const char *fs_save_device()
{
    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(2048);

    doc["update_url"] = update_url;

    doc["LOC"] = LOC;
    doc["TIP"] = TIP;
    doc["NAME"] = NAME;

    doc["heartbeat"] = heartbeat;
    doc["heartbeat_minutes"] = heartbeat_minutes;

    doc["start_webserver"] = start_webserver;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(dev_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    LittleFS.end();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ##     ##  #######  ######## ########
// ##    ##    ###   ### ##     ##    ##       ##
// ##          #### #### ##     ##    ##       ##
//  ######     ## ### ## ##     ##    ##       ##
//       ##    ##     ## ##  ## ##    ##       ##
// ##    ##    ##     ## ##    ##     ##       ##
//  ######     ##     ##  ##### ##    ##       ##

const char *fs_save_mqtt()
{
    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(512);

    doc["MQTT_HOST"] = MQTT_HOST;
    doc["MQTT_PORT"] = MQTT_PORT;
    doc["MQTT_USER"] = MQTT_USER;
    doc["MQTT_PASS"] = MQTT_PASS;
    doc["WILL"] = WILL;
    doc["PUB"] = PUB;
    doc["SUB"] = SUB;
    doc["STAT"] = STAT;
    doc["start_mqtt"] = start_mqtt;

    yield();

    // serialize & write config
    File configFile = LittleFS.open(mqtt_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    LittleFS.end();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######     ##       ####  ######   ##     ## ########  ######
// ##    ##    ##        ##  ##    ##  ##     ##    ##    ##    ##
// ##          ##        ##  ##        ##     ##    ##    ##
//  ######     ##        ##  ##   #### #########    ##     ######
//       ##    ##        ##  ##    ##  ##     ##    ##          ##
// ##    ##    ##        ##  ##    ##  ##     ##    ##    ##    ##
//  ######     ######## ####  ######   ##     ##    ##     ######

const char *fs_save_lights()
{

    if (!LittleFS.begin())
        return littlefs_failure;

    DynamicJsonDocument doc(2048);

    uint8_t i = 0;

    for (i = 0; i < 16; i++)
        doc["chn_to_mcp"].add(chn_to_mcp[i]);

    doc["lights"] = lights;
    for (i = 0; i < lights; i++)
        doc["light_chn"].add(light_chn[i]);

    doc["simple"] = simple;
    for (i = 0; i < simple; i++)
        doc["simple_chn"].add(simple_chn[i]);

    doc["shutters"] = shutters;
    for (i = 0; i < shutters; i++)
    {
        doc["shutter_chn_up"].add(shutter_chn_up[i]);
        doc["shutter_chn_down"].add(shutter_chn_down[i]);
        doc["shutter_timing"].add(shutter_timing[i]);
    }

    doc["timed"] = timed;
    for (i = 0; i < timed; i++)
    {
        doc["timed_chn"].add(timed_chn[i]);
        doc["timed_timing"].add(timed_timing[i]);
    }

    yield();

    // serialize & write config
    File configFile = LittleFS.open(lights_cfg_file, "w");
    if (!configFile)
        return file_write_failure;

    serializeJson(doc, configFile);
    configFile.close();

    doc.clear();

    LittleFS.end();

    return file_write_success;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  #######  ########  ##     ##    ###    ########       ########  ######
// ##       ##     ## ##     ## ###   ###   ## ##      ##          ##       ##    ##
// ##       ##     ## ##     ## #### ####  ##   ##     ##          ##       ##
// ######   ##     ## ########  ## ### ## ##     ##    ##          ######    ######
// ##       ##     ## ##   ##   ##     ## #########    ##          ##             ##
// ##       ##     ## ##    ##  ##     ## ##     ##    ##          ##       ##    ##
// ##        #######  ##     ## ##     ## ##     ##    ##          ##        ######

void fs_format()
{
    if (!LittleFS.begin())
        return;

    LittleFS.format();

    LittleFS.end();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
