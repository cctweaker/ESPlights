//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void check_MCP()
{
    Wire.beginTransmission(MCP_ADDR);

    if (Wire.endTransmission() == 0)
    {
        // MCP found!
        MCP = true;
        return;
    }

    MCP = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void setup_MCP()
{
    // set all to low
    Wire.beginTransmission(MCP_ADDR);
    Wire.write(GPIOA); // GPIOA
    Wire.write(0);     // GPIOA
    Wire.write(0);     // GPIOB
    Wire.write(0);     // OLATA
    Wire.write(0);     // OLATB
    Wire.endTransmission();

    // set all outputs
    Wire.beginTransmission(MCP_ADDR);
    Wire.write(0x00); // IODIRA address
    Wire.write(0);    // IODIRA
    Wire.write(0);    // IODIRB
    Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void update_pins(uint8_t pin, uint8_t value)
{
    if (pin == 0 || pin > 16)
        return;

    uint8_t pos = chn_to_mcp[pin - 1]; // position in 16bit (PORTB:PORTA MSB>LSB)

    if (value > 1)
        value = !bitRead(pins, pos);
    bitWrite(pins, pos, value);

    update_MCP();

    client.publish(MQTT_CHAN_TOPIC + String(pin), String(value), true, 0);

    if (do_save_lights)
    {
        client.publish(MQTT_SAVE_TOPIC, String(light_status), true, 0);
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

    Wire.beginTransmission(MCP_ADDR);
    Wire.write(GPIOA);
    Wire.write(portA);
    Wire.write(portB);
    Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void init_lights()
{
    do_init_lights = false;

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
            if ((unsigned long)(millis() - shutter_timeout[i]) > shutter_timing[i] * 1000)
            {
                client.publish(MQTT_SHUT_TOPIC + String(i + 1), String(shutter_status[i]), true, 0);
                update_pins(shutter_chn_up[i], 0);
                update_pins(shutter_chn_down[i], 0);
                shutter_timeout[i] = 0;
                shutter_status[i] = 0;
            }
        }
    }

    /////////////////////////

    for (i = 0; i < doors; i++) // test all door timeouts
    {
        if (door_timeout[i] > 0)
        {
            if ((unsigned long)(millis() - door_timeout[i]) > door_timing[i] * 1000)
            {
                update_pins(door_chn[i], 0);
                door_timeout[i] = 0;
            }
        }
    }

    /////////////////////////

    for (i = 0; i < gates; i++) // test all gate timeouts
    {
        if (gate_timeout[i] > 0)
        {
            if ((unsigned long)(millis() - gate_timeout[i]) > gate_timing[i])
            {
                update_pins(gate_chn[i], 0);
                gate_timeout[i] = 0;
            }
        }
    }
}