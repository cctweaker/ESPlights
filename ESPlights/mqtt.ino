
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// #### ##    ## #### ########    ##     ##  #######  ######## ########
//  ##  ###   ##  ##     ##       ###   ### ##     ##    ##       ##
//  ##  ####  ##  ##     ##       #### #### ##     ##    ##       ##
//  ##  ## ## ##  ##     ##       ## ### ## ##     ##    ##       ##
//  ##  ##  ####  ##     ##       ##     ## ##  ## ##    ##       ##
//  ##  ##   ###  ##     ##       ##     ## ##    ##     ##       ##
// #### ##    ## ####    ##       ##     ##  ##### ##    ##       ##

void init_mqtt()
{
    if (!start_mqtt || !use_wifi)
        return;

    char topic[128];
    sprintf(topic, "%s%s%s%s/%s", LOC, TIP, NAME, XTRA, WILL);

    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.setWill(topic, "0", true, 0);
    client.onMessage(messageReceived);

    connect_mqtt();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######   #######  ##    ## ##    ## ########  ######  ########    ##     ##  #######  ######## ########
// ##    ## ##     ## ###   ## ###   ## ##       ##    ##    ##       ###   ### ##     ##    ##       ##
// ##       ##     ## ####  ## ####  ## ##       ##          ##       #### #### ##     ##    ##       ##
// ##       ##     ## ## ## ## ## ## ## ######   ##          ##       ## ### ## ##     ##    ##       ##
// ##       ##     ## ##  #### ##  #### ##       ##          ##       ##     ## ##  ## ##    ##       ##
// ##    ## ##     ## ##   ### ##   ### ##       ##    ##    ##       ##     ## ##    ##     ##       ##
//  ######   #######  ##    ## ##    ## ########  ######     ##       ##     ##  ##### ##    ##       ##

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

    sprintf(topic, "%s%s%s%s/%s/+", LOC, TIP, NAME, XTRA, SUB);
    client.subscribe(topic, 0);

    sprintf(topic, "%s%s%s%s/%s", LOC, TIP, NAME, XTRA, WILL);
    client.publish(topic, "1", true, 0);

    mqtt_heartbeat();

    use_mqtt = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##        #######   #######  ########
// ##       ##     ## ##     ## ##     ##
// ##       ##     ## ##     ## ##     ##
// ##       ##     ## ##     ## ########
// ##       ##     ## ##     ## ##
// ##       ##     ## ##     ## ##
// ########  #######   #######  ##

void mqtt_loop()
{
    if (heartbeat)
        if ((unsigned long)(millis() - last_heartbeat) > heartbeat_minutes * 60 * 1000)
        {
            last_heartbeat = millis();
            mqtt_heartbeat();
        }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ########  ##     ##
// ##     ##  ##   ##
// ##     ##   ## ##
// ########     ###
// ##   ##     ## ##
// ##    ##   ##   ##
// ##     ## ##     ##

void messageReceived(String &topic, String &payload)
{
    uint8_t idx = topic.lastIndexOf('/') + 1;
    String cmnd = topic.substring(idx);
    uint8_t value = payload.toInt();

    if (!pin_states_known && cmnd == "saved")
    {
        pin_states_known = true;
        light_status = payload.toInt();
        untimed_status = light_status;
        init_lights();
        return;
    }

    if (cmnd == "all")
    {
        if (value && shutters == 0 && lights == max_channels) // if payload > 0 and no shutter and all channels are lights then permit all on
        {
            light_status = 65535;
            untimed_status = 65535;
            do_save_lights = true;
            for (uint8_t i = 0; i < max_channels; i++)
                update_pins(light_chn[i], 1);
        }
        if (value == 0)
        {
            light_status = 0;
            untimed_status = 0;
            do_save_lights = true;
            for (uint8_t i = 0; i < max_channels; i++)
                update_pins(light_chn[i], 0);
        }
    }

    if (cmnd == "reset")
    {
        ESP.restart();
    }

    if (cmnd == "webserver")
    {
        if (value)
            start_webserver = true;
        else
            start_webserver = false;

        fs_save_device();
    }

    if (cmnd == "update")
    {
        do_ota_update = true;
        return;
    }

    /////////////////

    cmnd = topic.substring(idx, idx + 1);
    String chns = topic.substring(idx + 1);
    uint8_t chn = chns.toInt() - 1;

    if (cmnd == "s") // shutters
    {
        if (chn >= shutters)
            return;

        char topic[128];

        sprintf(topic, "%s%s%s%s/shutter/%d", LOC, TIP, NAME, XTRA, chn + 1);
        client.publish(topic, "0", true, 0);

        if (shutter_status[chn] == 0)
        {
            if (value == 2)
            {
                shutter_status[chn] = 2;
                update_pins(shutter_chn_up[chn], 1);
                shutter_timeout[chn] = millis();
            }
            if (value == 1)
            {
                shutter_status[chn] = 1;
                update_pins(shutter_chn_down[chn], 1);
                shutter_timeout[chn] = millis();
            }
        }
        else
        {
            update_pins(shutter_chn_up[chn], 0);
            update_pins(shutter_chn_down[chn], 0);
            shutter_status[chn] = 0;
            shutter_timeout[chn] = 0;
        }
    }

    if (cmnd == "t") // timed
    {
        if (chn >= timed)
            return;

        if (bitRead(untimed_status, timed_chn[chn] - 1))
            return;

        if (value == 0)
        {
            timed_timeout[chn] = 0;
            update_pins(timed_chn[chn], 0);
        }
        else if (value == 1)
        {
            timed_timeout[chn] = millis();
            update_pins(timed_chn[chn], 1);
        }
        else
        {
            uint8_t pos = 0;
            if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
                pos = chn_to_expander[timed_chn[chn] - 1];
            else
                pos = timed_chn[chn] - 1;

            uint8_t prev = bitRead(pins, pos);

            if (prev == 1)
            {
                timed_timeout[chn] = 0;
                update_pins(timed_chn[chn], 0);
            }
            else
            {
                timed_timeout[chn] = millis();
                update_pins(timed_chn[chn], 1);
            }
        }
    }

    if (cmnd == "l") // light channels
    {
        if (chn >= lights)
            return;

        do_save_lights = true;

        if (value > 1)
            value = !bitRead(light_status, light_chn[chn] - 1);

        bitWrite(light_status, light_chn[chn] - 1, value);

        bitWrite(untimed_status, light_chn[chn] - 1, value);

        update_pins(light_chn[chn], value);
    }

    if (cmnd == "n") // simple channels
    {
        if (chn >= simple)
            return;

        if (value == 0)
            if (bitRead(light_status, simple_chn[chn] - 1))
            {
                bitWrite(light_status, simple_chn[chn] - 1, value);
                do_save_lights = true;
            }

        bitWrite(untimed_status, simple_chn[chn] - 1, value);

        update_pins(simple_chn[chn], value);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ## ########    ###    ########  ######## ########  ########    ###    ########
// ##     ## ##         ## ##   ##     ##    ##    ##     ## ##         ## ##      ##
// ##     ## ##        ##   ##  ##     ##    ##    ##     ## ##        ##   ##     ##
// ######### ######   ##     ## ########     ##    ########  ######   ##     ##    ##
// ##     ## ##       ######### ##   ##      ##    ##     ## ##       #########    ##
// ##     ## ##       ##     ## ##    ##     ##    ##     ## ##       ##     ##    ##
// ##     ## ######## ##     ## ##     ##    ##    ########  ######## ##     ##    ##

void mqtt_heartbeat()
{
    char topic[128];
    char mqtt_tx[256];

    DynamicJsonDocument doc(256);

#ifndef USE_SSL
    doc["fw"] = FW_NAME;
#else
    doc["fw"] = String(FW_NAME) + "_SSL";
#endif
    doc["ver"] = VERSION;
    doc["ID"] = String(ESP.getChipId(), HEX);
    doc["Vcc"] = ESP.getVcc();
    doc["SSID"] = WiFi.SSID();
    doc["RSSI"] = WiFi.RSSI();
    doc["BSSID"] = WiFi.BSSIDstr();
    doc["IP"] = WiFi.localIP().toString();

    serializeJson(doc, mqtt_tx);
    doc.clear();
    sprintf(topic, "%s%s%s%s/%s", LOC, TIP, NAME, XTRA, STAT);
    client.publish(topic, mqtt_tx, true, 0);

    doc["free heap"] = ESP.getFreeHeap();
    doc["heap frag"] = ESP.getHeapFragmentation();
    doc["max block"] = ESP.getMaxFreeBlockSize();
    doc["core v"] = ESP.getCoreVersion();
    doc["sdk v"] = ESP.getSdkVersion();
    doc["sketch"] = ESP.getSketchSize();
    doc["sketch free"] = ESP.getFreeSketchSpace();
    doc["MD5"] = ESP.getSketchMD5();
    doc["crc"] = ESP.checkFlashCRC();

    serializeJson(doc, mqtt_tx);
    doc.clear();
    sprintf(topic, "%s%s%s%s/%s/1", LOC, TIP, NAME, XTRA, STAT);
    client.publish(topic, mqtt_tx, true, 0);

    doc["CPU freq"] = ESP.getCpuFreqMHz();
    doc["flash freq"] = ESP.getFlashChipSpeed() / 1000000;
    doc["flash ID"] = String(ESP.getFlashChipId(), HEX);
    doc["flash size"] = ESP.getFlashChipSize();
    doc["real flash size"] = ESP.getFlashChipRealSize();
    doc["CPU cycles"] = ESP.getCycleCount();

    serializeJson(doc, mqtt_tx);
    doc.clear();
    sprintf(topic, "%s%s%s%s/%s/2", LOC, TIP, NAME, XTRA, STAT);
    client.publish(topic, mqtt_tx, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
