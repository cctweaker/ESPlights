
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ## ########  ######  ##    ##    ######## ##     ## ########     ###    ##    ## ########  ######## ########
// ##    ## ##     ## ##       ##    ## ##   ##     ##        ##   ##  ##     ##   ## ##   ###   ## ##     ## ##       ##     ##
// ##       ##     ## ##       ##       ##  ##      ##         ## ##   ##     ##  ##   ##  ####  ## ##     ## ##       ##     ##
// ##       ######### ######   ##       #####       ######      ###    ########  ##     ## ## ## ## ##     ## ######   ########
// ##       ##     ## ##       ##       ##  ##      ##         ## ##   ##        ######### ##  #### ##     ## ##       ##   ##
// ##    ## ##     ## ##       ##    ## ##   ##     ##        ##   ##  ##        ##     ## ##   ### ##     ## ##       ##    ##
//  ######  ##     ## ########  ######  ##    ##    ######## ##     ## ##        ##     ## ##    ## ########  ######## ##     ##

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
// #### ##    ## #### ########    ######## ##     ## ########     ###    ##    ## ########  ######## ########
//  ##  ###   ##  ##     ##       ##        ##   ##  ##     ##   ## ##   ###   ## ##     ## ##       ##     ##
//  ##  ####  ##  ##     ##       ##         ## ##   ##     ##  ##   ##  ####  ## ##     ## ##       ##     ##
//  ##  ## ## ##  ##     ##       ######      ###    ########  ##     ## ## ## ## ##     ## ######   ########
//  ##  ##  ####  ##     ##       ##         ## ##   ##        ######### ##  #### ##     ## ##       ##   ##
//  ##  ##   ###  ##     ##       ##        ##   ##  ##        ##     ## ##   ### ##     ## ##       ##    ##
// #### ##    ## ####    ##       ######## ##     ## ##        ##     ## ##    ## ########  ######## ##     ##

void init_expander()
{
    Wire.begin(2, 0); // connect SDA to GPIO2 and SCL to GPIO0
    check_expander_present();

    if (use_lights)
    {
        uint8_t byte = 0x00;

        // init MCP23017 IO expander
        if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        {
            // set all to low
            Wire.beginTransmission(io_expander_address);
            Wire.write(0x12); // register address for GPIOA
            Wire.write(byte); // GPIOA (0x12)
            Wire.write(byte); // GPIOB (0x13)
            Wire.write(byte); // OLATA (0x14)
            Wire.write(byte); // OLATB (0x15)
            Wire.endTransmission();

            if (is_relay)
                byte = 0xFF;

            // set all outputs
            Wire.beginTransmission(io_expander_address);
            Wire.write(0x00); // register address for IODIRA
            Wire.write(byte); // IODIRA (0x00), all outputs
            Wire.write(byte); // IODIRB (0x01), all outputs
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
// #### ##    ## #### ########    ##       ####  ######   ##     ## ########  ######
//  ##  ###   ##  ##     ##       ##        ##  ##    ##  ##     ##    ##    ##    ##
//  ##  ####  ##  ##     ##       ##        ##  ##        ##     ##    ##    ##
//  ##  ## ## ##  ##     ##       ##        ##  ##   #### #########    ##     ######
//  ##  ##  ####  ##     ##       ##        ##  ##    ##  ##     ##    ##          ##
//  ##  ##   ###  ##     ##       ##        ##  ##    ##  ##     ##    ##    ##    ##
// #### ##    ## ####    ##       ######## ####  ######   ##     ##    ##     ######

void init_lights()
{
    uint8_t value = 0;

    for (uint8_t i = 0; i < max_channels; i++)
    {
        value = bitRead(light_status, i);
        if (value)
            update_pins(i + 1, value);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ## ########  ########     ###    ######## ########    ########  #### ##    ##  ######
// ##     ## ##     ## ##     ##   ## ##      ##    ##          ##     ##  ##  ###   ## ##    ##
// ##     ## ##     ## ##     ##  ##   ##     ##    ##          ##     ##  ##  ####  ## ##
// ##     ## ########  ##     ## ##     ##    ##    ######      ########   ##  ## ## ##  ######
// ##     ## ##        ##     ## #########    ##    ##          ##         ##  ##  ####       ##
// ##     ## ##        ##     ## ##     ##    ##    ##          ##         ##  ##   ### ##    ##
//  #######  ##        ########  ##     ##    ##    ########    ##        #### ##    ##  ######

void update_pins(uint8_t pin, uint8_t value)
{
    if (pin == 0 || pin > 16)
        return;

    uint8_t pos = 0;

    if (io_expander_address >= 0x20 && io_expander_address <= 0x27)
        // we have MCP23017 expander
        pos = chn_to_expander[pin - 1]; // position in 16bit (PORTB:PORTA MSB>LSB)
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

    sprintf(topic, "%s%s%s%s/channel/%d", LOC, TIP, NAME, XTRA, pin);
    client.publish(topic, String(value), true, 0);

    if (do_save_lights)
    {
        sprintf(topic, "%s%s%s%s/%s/saved", LOC, TIP, NAME, XTRA, SUB);
        client.publish(topic, String(light_status), true, 0);
        do_save_lights = false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ## ########  ########     ###    ######## ########    ##     ##  ######  ########
// ##     ## ##     ## ##     ##   ## ##      ##    ##          ###   ### ##    ## ##     ##
// ##     ## ##     ## ##     ##  ##   ##     ##    ##          #### #### ##       ##     ##
// ##     ## ########  ##     ## ##     ##    ##    ######      ## ### ## ##       ########
// ##     ## ##        ##     ## #########    ##    ##          ##     ## ##       ##
// ##     ## ##        ##     ## ##     ##    ##    ##          ##     ## ##    ## ##
//  #######  ##        ########  ##     ##    ##    ########    ##     ##  ######  ##

void update_MCP()
{
    uint16_t pini = pins;

    if (is_relay)
        pini = ~pins;

    uint8_t portA = pini;
    uint8_t portB = pini >> 8;

    if (is_relay)
    {
        Wire.beginTransmission(io_expander_address);
        Wire.write(0x00);  // register address for IODIRA
        Wire.write(portA); // IODIRA
        Wire.write(portB); // IODIRB
        Wire.endTransmission();

        Wire.beginTransmission(io_expander_address);
        Wire.write(0x12); // register address for GPIOA
        Wire.write(0x00);
        Wire.write(0x00);
        Wire.endTransmission();
    }
    else
    {
        Wire.beginTransmission(io_expander_address);
        Wire.write(0x12); // register address for GPIOA
        Wire.write(portA);
        Wire.write(portB);
        Wire.endTransmission();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// ##     ## ########  ########     ###    ######## ########    ########   ######     ###
// ##     ## ##     ## ##     ##   ## ##      ##    ##          ##     ## ##    ##   ## ##
// ##     ## ##     ## ##     ##  ##   ##     ##    ##          ##     ## ##        ##   ##
// ##     ## ########  ##     ## ##     ##    ##    ######      ########  ##       ##     ##
// ##     ## ##        ##     ## #########    ##    ##          ##        ##       #########
// ##     ## ##        ##     ## ##     ##    ##    ##          ##        ##    ## ##     ##
//  #######  ##        ########  ##     ##    ##    ########    ##         ######  ##     ##

void update_PCA()
{
    uint8_t pca_pins = ~pins;

    if (is_relay)
        pca_pins = pins;

    Wire.beginTransmission(io_expander_address);
    Wire.write(1);
    Wire.write(pca_pins);
    Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ######  ##     ## ########  ######  ##    ##    ######## #### ##     ## ######## ########   ######
// ##    ## ##     ## ##       ##    ## ##   ##        ##     ##  ###   ### ##       ##     ## ##    ##
// ##       ##     ## ##       ##       ##  ##         ##     ##  #### #### ##       ##     ## ##
// ##       ######### ######   ##       #####          ##     ##  ## ### ## ######   ########   ######
// ##       ##     ## ##       ##       ##  ##         ##     ##  ##     ## ##       ##   ##         ##
// ##    ## ##     ## ##       ##    ## ##   ##        ##     ##  ##     ## ##       ##    ##  ##    ##
//  ######  ##     ## ########  ######  ##    ##       ##    #### ##     ## ######## ##     ##  ######

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

                sprintf(topic, "%s%s%s%s/shutter/%d", LOC, TIP, NAME, XTRA, i + 1);
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
                timed_timeout[i] = 0;

                if (!bitRead(untimed_status, timed_chn[i] - 1))
                    update_pins(timed_chn[i], 0);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////