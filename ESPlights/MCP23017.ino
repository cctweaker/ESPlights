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

void update_pins(uint8_t pin, uint8_t value)
{
    if (pin == 0 || pin > 16)
        return;

    uint8_t pos = 16 - pin; // position in 16bit pin (PORTB:PORTA MSB>LSB)

    if (value > 1)
        value = !bitRead(pins, pos);
    bitWrite(pins, pos, value);

    update_MCP();

    client.publish(MQTT_CHAN_TOPIC + String(pin), String(value), true, 0);
    client.publish(MQTT_SAVE_TOPIC, String(pins), true, 0);
}

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