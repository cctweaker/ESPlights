//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void mqtt_setup()
{
    client.begin(MQTT_HOST, MQTT_PORT, net);
    client.setWill(MQTT_WILL_TOPIC, "0", true, 0);
    client.onMessage(messageReceived);
    mqtt_connect();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void mqtt_connect()
{
    uint8_t i = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        i++;
        if (i > 100)
            ESP.restart();
    }

    // certificare
    BearSSL::X509List cert(digicert);
    net.setTrustAnchors(&cert);

    while (!client.connect(HOSTNAME, MQTT_USER, MQTT_PASS))
    {
        delay(100);
        i++;
        if (i > 100)
            ESP.restart();
    }

    client.subscribe(MQTT_SAVE_TOPIC, 0);
    client.publish(MQTT_WILL_TOPIC, "1", true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void messageReceived(String &topic, String &payload)
{
    if (!pin_states_known)
    {
        client.unsubscribe(MQTT_SAVE_TOPIC);
        client.subscribe(MQTT_CMND_TOPIC, 0);
        pin_states_known = true;
        do_init_lights = true;
        light_status = payload.toInt();
        return;
    }

    uint8_t idx = topic.lastIndexOf('/') + 1;
    String cmnd = topic.substring(idx);

    if (cmnd == "save")
    {
        do_save_config = true;
        return;
    }

    if (cmnd == "update")
    {
        do_ota_update = true;
        return;
    }

    if (cmnd == "reset")
    {
        ESP.restart();
    }

    if (cmnd == "format")
    {
        do_fs_format = true;
        return;
    }

    if (cmnd == "list")
    {
        do_list_config = true;
        return;
    }

    /////////////////

    if (cmnd == "config")
    {
        uint8_t i = 0;

        StaticJsonDocument<1024> doc;
        deserializeJson(doc, payload);

        if (doc.containsKey("TIP"))
            strlcpy(TIP, doc["TIP"], sizeof(TIP));
        if (doc.containsKey("LOC"))
            strlcpy(LOC, doc["LOC"], sizeof(LOC));

        if (doc.containsKey("SSIDa"))
            strlcpy(SSIDa, doc["SSIDa"], sizeof(SSIDa));
        if (doc.containsKey("PASSa"))
            strlcpy(PASSa, doc["PASSa"], sizeof(PASSa));
        if (doc.containsKey("SSIDb"))
            strlcpy(SSIDb, doc["SSIDb"], sizeof(SSIDb));
        if (doc.containsKey("PASSb"))
            strlcpy(PASSb, doc["PASSb"], sizeof(PASSb));

        if (doc.containsKey("MQTT_HOST"))
            strlcpy(MQTT_HOST, doc["MQTT_HOST"], sizeof(MQTT_HOST));
        if (doc.containsKey("MQTT_PORT"))
            MQTT_PORT = doc["MQTT_PORT"];
        if (doc.containsKey("MQTT_USER"))
            strlcpy(MQTT_USER, doc["MQTT_USER"], sizeof(MQTT_USER));
        if (doc.containsKey("MQTT_PASS"))
            strlcpy(MQTT_PASS, doc["MQTT_PASS"], sizeof(MQTT_PASS));

        if (doc.containsKey("update_url"))
            strlcpy(update_url, doc["update_url"], sizeof(update_url));

        if (doc.containsKey("FACTOR"))
            FACTOR = doc["FACTOR"];
        if (doc.containsKey("heartbeat"))
            heartbeat = doc["heartbeat"];
        if (doc.containsKey("heartbeat_minutes"))
            heartbeat_minutes = doc["heartbeat_minutes"];

        if (doc.containsKey("chn_to_mcp"))
            for (i = 0; i < 16; i++)
                chn_to_mcp[i] = doc["chn_to_mcp"][i];

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

        if (doc.containsKey("doors"))
            doors = doc["doors"];
        if (doc.containsKey("door_chn"))
            for (i = 0; i < doors; i++)
                door_chn[i] = doc["door_chn"][i];
        if (doc.containsKey("door_timing"))
            for (i = 0; i < doors; i++)
                door_timing[i] = doc["door_timing"][i];

        if (doc.containsKey("gates"))
            gates = doc["gates"];
        if (doc.containsKey("gate_chn"))
            for (i = 0; i < gates; i++)
                gate_chn[i] = doc["gate_chn"][i];
        if (doc.containsKey("gate_timing"))
            for (i = 0; i < gates; i++)
                gate_timing[i] = doc["gate_timing"][i];

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

        doc.clear();
        init_others();
        return;
    }

    /////////////////

    cmnd = topic.substring(idx, idx + 1);
    String chns = topic.substring(idx + 1);
    uint8_t chn = chns.toInt() - 1;
    uint8_t value = payload.toInt();

    if (cmnd == "s") // shutters
    {
        if (chn >= shutters)
            return;

        client.publish(MQTT_SHUT_TOPIC + String(chn + 1), "0", true, 0);

        if (shutter_status[chn] == 0)
        {
            if (value == SHUTTER_UP)
            {
                shutter_status[chn] = SHUTTER_UP;
                update_pins(shutter_chn_up[chn], 1);
                shutter_timeout[chn] = millis();
            }
            if (value == SHUTTER_DOWN)
            {
                shutter_status[chn] = SHUTTER_DOWN;
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

    if (cmnd == "d") // doors
    {
        if (chn >= doors)
            return;

        door_timeout[chn] = millis();
        update_pins(door_chn[chn], 1);
    }

    if (cmnd == "g") // gates
    {
        if (chn >= gates)
            return;

        gate_timeout[chn] = millis();
        update_pins(gate_chn[chn], 1);
    }

    if (cmnd == "l") // light channels
    {
        if (chn >= lights)
            return;

        do_save_lights = true;
        if (value > 1)
            value = !bitRead(light_status, chn);
        bitWrite(light_status, chn, value);

        update_pins(light_chn[chn], value);
    }

    if (cmnd == "n") // simple channels
    {
        if (chn >= simple)
            return;

        update_pins(simple_chn[chn], value);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_mac_topics()
{
    sprintf(HOSTNAME, "%s_%x", TIP, ESP.getChipId());
    sprintf(MQTT_WILL_TOPIC, "%s/%s_%x%s", LOC, TIP, ESP.getChipId(), WILL);
    sprintf(MQTT_CHAN_TOPIC, "%s/%s_%x%s", LOC, TIP, ESP.getChipId(), CHAN);
    sprintf(MQTT_CMND_TOPIC, "%s/%s_%x%s", LOC, TIP, ESP.getChipId(), CMND);
    sprintf(MQTT_STAT_TOPIC, "%s/%s_%x%s", LOC, TIP, ESP.getChipId(), STAT);
    sprintf(MQTT_SAVE_TOPIC, "%s/%s_%x%s", LOC, TIP, ESP.getChipId(), SAVE);
    sprintf(MQTT_SHUT_TOPIC, "%s/%s_%x%s", LOC, TIP, ESP.getChipId(), SHUT);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void send_heartbeat()
{
    char tx[256];
    float supply = (float)ESP.getVcc() / FACTOR;

    sprintf(tx, "{\"Type\":\"%s\",\"ID\":\"%x\",\"Vin\":%.2f,\"SSID\":\"%s\",\"RSSI\":%d,\"BSSID\":\"%s\"}", TIP, ESP.getChipId(), supply, WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.BSSIDstr().c_str());

    client.publish(MQTT_STAT_TOPIC, tx, true, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
