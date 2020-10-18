//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void update_pins(uint8_t pin, uint8_t value)
{
    if (pin == 0 || pin > 16)
        return;

    uint8_t pos = 0;

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        // we have MCP23017 expander
        pos = chn_to_mcp[pin - 1]; // position in 16bit (PORTB:PORTA MSB>LSB)
    else
        // we have PCA expander
        pos = pin - 1;

    if (value > 1)
        value = !bitRead(pins, pos);
    bitWrite(pins, pos, value);

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        update_MCP();
    else
        update_PCA();

    // MQTT publishing of status

    char topic[128];

    sprintf(topic, "%s/%s/%s/channel/%d", LOC, TIP, NAME, pin);
    client.publish(topic, String(value), true, 0);

    if (do_save_lights)
    {
        sprintf(topic, "%s/%s/%s/%s/saved", LOC, TIP, NAME, SUB);
        client.publish(topic, String(light_status), true, 0);
        do_save_lights = false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void update_MCP()
{
    uint8_t portA = pins;
    uint8_t portB = pins >> 8;

    Wire.beginTransmission(io_expander_address);
    Wire.write(0x12); // register address for GPIOA
    Wire.write(portA);
    Wire.write(portB);
    Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void update_PCA()
{
    uint8_t pca_pins = ~pins;

    Wire.beginTransmission(io_expander_address);
    Wire.write(1);
    Wire.write(pca_pins);
    Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_lights()
{
    uint8_t value = 0;

    for (uint8_t i = 0; i < lights; i++)
    {
        value = bitRead(light_status, i);
        if (value)
            update_pins(light_chn[i], value);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void check_timers()
{
    uint8_t i = 0;

    for (i = 0; i < shutters; i++) // test all shutter timeouts
    {
        if (shutter_timeout[i] > 0)
        {
            if ((unsigned long)(millis() - shutter_timeout[i]) > shutter_timing[i])
            {
                char topic[128];

                sprintf(topic, "%s/%s/%s/shutter/%d", LOC, TIP, NAME, i + 1);
                client.publish(topic, String(shutter_status[i]), true, 0);

                update_pins(shutter_chn_up[i], 0);
                update_pins(shutter_chn_down[i], 0);
                shutter_timeout[i] = 0;
                shutter_status[i] = 0;
            }
        }
    }

    /////////////////////////

    for (i = 0; i < timed; i++) // test all timed channels timeouts
    {
        if (timed_timeout[i] > 0)
        {
            if ((unsigned long)(millis() - timed_timeout[i]) > timed_timing[i])
            {
                update_pins(timed_chn[i], 0);
                timed_timeout[i] = 0;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////