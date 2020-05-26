# ESPlights
Software for the 16 channel mains controller provided by <a href="https://3dstar.ro/proiecte/esplights">3DStar.ro</a>

# Features
- secure MQTT connection, Let's Ecrypt CA cetificate already loaded
- topic structure for multiple buildings (locations) control: <code>location/device_espid/+</code>
- lights status saved and recovered on restart from MQTT server
- on / off / toggle commands for each light and simple channel
- shutter/blinds control with up/stop/down/stop commands and max timeout per shutter (in seconds)
- current shutter status topic based on timeout for end of travel determination
- door control with max timeout per door (in seconds)
- gate control with max timeout per gate (in milliseconds, good for impulses)
- simple channels, they work like lights channels but status is lost on restart
- separate status topic for each channel to show current channel state
- main and backup WiFi connections
- secured, no access point is presented in case both WiFi connections are unavailable
- no web access, just pure MQTT
- extra information on device status topic: type, ID, supply voltage, AP SSID, AP RSSI, AP mac



# Hardware
Roll your own or get one from <a href="https://3dstar.ro/proiecte/esplights">3DStar shop</a> in PCB, kit (PCB + components), SMD kit (PCB with soldered SMD components + bags of through hole components) or fully assembled form.

The ESP-01 / ESP-01S from AI Thinker is used to control one MCP23017 16bit IO expander. On each channel there are 1 optotriac and 1 triac to control mains powered lights or other devices. Each channel can switch up to around 600W without any special cooling.

The controller must be placed after a 10A circuit breaker so total power should not exceed 2300W in Europe. Channels up to 600W can be turned on if maximum total power draw is respected. These informations are general guidelines, please conform to your local regulations!
If all channels are used equally, this corresponds to about 137W per channel, enough for 9 x 15W LED bulbs per each channel or a total of 144 15W LED bulbs.
The controller should cover most houses and apartments without problems, leaving a few extra channels for shutters/blinds, doors/gates, irrigation or other controls.
The number of boards you can use is unlimited so you can controll all your mains powered small devices.


# Build & upload
- there are 2 methods:
    - download, edit config.h, build & upload
    - download, edit config.json, build & upload main program then also upload file system image
- in both cases PlatformIO is recommended for automatic library management

# Usage
- mqtt topic: <code>LOC/TIP_ESPid</code> where LOC & TIP are user defined and ESPid is the HEX representation of your ESP mac address' last 3 bytes
- command topic: <code>/cmnd/...</code>
    - <code>save</code>: save current configuration to LittleFS
    - <code>update</code>: perform OTA update
    - <code>reset</code>: restart ESP device
    - <code>format</code>: format LittleFS space
    - <code>list</code>: list current LittleFS config.json to Serial @ 115200bps. Other than this serial is not used at all.
    - <code>config: change device configuration. Due to MQTT message length restrictions do not send json messages longer than 256 bytes. Get examples from config.json in data folde. You can send any part at any time. Remember to send save command after you are satisfied with the configuration.

    - <code>/sX</code>: shutter control. X is the shutter number. Payload can be 0 = stop, 1 = UP, 2 = down. Repeating UP/DOWN also stops the shutters.
    - <code>/dX</code>: door control. X is the door number. Payload can be 0 = off, 1 = on, 2 = toggle
    - <code>/gX</code>: gate control. X is the gate number. Payload can be 0 = off, 1 = on, 2 = toggle
    - <code>/lX</code>: lights control. X is the light number. Payload can be 0 = off, 1 = on, 2 = toggle
    - <code>/nX</code>: simple/normal control. X is the number. Payload can be 0 = off, 1 = on, 2 = toggle

- channel status topic: <code>/chan/X</code> where X is the channel number. Payload will be 0 for OFF and 1 for ON. It is updated by the ESP after processing commands, statuses are sent with retain flag.
- shutter status topic: <code>/shut/X</code> where X is the shutter number. It holds the current shutter positions. Payload will be 0 of unknown, 1 for UP, 2 for DOWN.
- status topic: <code>/stat</code> where the ESP sends periodic updates (heartbeat) with type, ID, supply voltage, SSID, RSSI, BSSID (AP mac) Example: <code>{"Type":"lights","ID":"abcdef","Vin":3.31,"SSID":"WIFI","RSSI":-29,"BSSID":"D8:50:E6:XX:XX:XX"}</code>
- will topic: <code>/will</code> Payload is 1 when device is ON and 0 when powered OFF
- save topic: <code>/save</code> where the device will save a retained message with its last state for lights. At startup it will load lights statuses from here and start the lights accordingly.

# Libraries
- https://github.com/256dpi/arduino-mqtt
- https://github.com/bblanchon/ArduinoJson

# PCB example
<img src="https://github.com/cctweaker/esplights/blob/master/Hardware/ESPlights v1.0 top example.jpg?raw=true">
<img src="https://github.com/cctweaker/esplights/blob/master/Hardware/ESPlights v1.0 bottom example.jpg?raw=true">