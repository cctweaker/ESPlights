# ESPlights
Software for the 16(8 or 4) channel mains controller provided by <a href="https://3dstar.ro/proiecte/esplights">3DStar.ro</a>

# Features
- secure MQTT connection possible, Let's Ecrypt CA cetificate already loaded
- topic structure for multiple buildings (locations) control: <code>location/device_type/device_name/</code>
- lights status saved and recovered on restart from MQTT server
- on / off / toggle commands for each light channel, simple channel and timed channel
- shutter/blinds control with up/stop/down/stop commands and max timeout per shutter
- current shutter status topic based on timeout for end of travel determination
- timed channels for bathroom ventilators, door access control, gates control, etc. with max timeout per channel (timeout freely adjustable from milliseconds to days)
- simple channels, they work like lights channels but status is lost on restart
- separate status topic for each channel to show current channel state
- main and backup WiFi connections
- secured, no access point is presented in case both WiFi connections are unavailable
- web interface for setup, can be disabled
- extra information on device status topic: type, ID, supply voltage, AP SSID, AP RSSI, AP mac, etc.



# Hardware
Roll your own or get one from <a href="https://3dstar.ro/proiecte/esplights">3DStar shop</a> in PCB, kit (PCB + components), SMD kit (PCB with soldered SMD components + bags of through hole components) or fully assembled form.

The ESP-01 / ESP-01S from AI Thinker is used to control a IO expander. Different types of IO expanders are supported: MCP23017 for 16 channels, PCA9557 for 8 channels and PCA9536 for 4 channels. On each channel there are 1 optotriac and 1 triac to control mains powered lights or other low powered devices. Each channel can switch up to around 300W without any special cooling.

The controller must be placed after a 10A circuit breaker so total power should not exceed 2300W in Europe. Channels up to 300W can be controlled if maximum total power draw is respected. These informations are general guidelines, please conform to your local regulations!
If all channels are used equally, this corresponds to about 137W per channel, enough for 9 x 15W LED bulbs per each channel or a total of 144 15W LED bulbs.
The controller should cover most houses and apartments without problems, leaving a few extra channels for shutters/blinds, doors/gates, irrigation or other controls.
The number of boards you can use is unlimited so you can controll all your mains powered small devices.


# Build & upload
- there are 2 methods:
    - clone, build & upload (PlatformIO is recommended for automatic library management)
    - download bin file, upload

# Setup
On the first powerup a WiFi access point is presented. If using the default configuration it is called "lights_ctrl_XXXXXX". Default password is abcd1234. Connect to it and you should arrive at the configuration page. If you are not automagically redirected (it's called a sign in page) then you can point your browser to http://192.168.10.1/

- [Device] menu will allow you to change LOC, TIP, NAME variables (these make up the MQTT topic), update URL (if you want to set up your own server for updates), MQTT heartbeat on/off and period and if the webserver should be started.
- [WIFI] menu will let you scan for WiFi networks and set up credentials for both networks. The second one is recommended to be set to your phone's hotspot in case you change the main Wifi network credentials.
- [MQTT] menu is used to set up your MQTT connection
- [Channels] menu will let you configure the devices channels. You can assign channels to lights, simple channels, shutter and timed channels. You can set up all the timeouts on shutter and timed channels.
- [Update firmware] menu is self explanatory
- [Erase settings] menu will delete all configurations, the device will be like new.
- [Restart] menu reboots the device
- [System information] menu will show some infos about the device.

# Usage
- MQTT topic: <code>LOC/TIP/NAME</code> where LOC, TIP & NAME are user defined. What follows are possible suffixes and they will be added after the mqtt topic.
- command topic: <code>/cmnd/...</code>
    - <code>/cmnd/saved</code>: saves the current lights status
    - <code>/update</code>: perform OTA update
    - <code>/reset</code>: restart ESP device
    
    - <code>/cmnd/lX</code>: lights control. X is the light channel number. Payload can be 0 = off, 1 = on, 2 = toggle
    - <code>/cmnd/nX</code>: simple/normal control. X is the number. Payload can be 0 = off, 1 = on, 2 = toggle
    - <code>/cmnd/sX</code>: shutter control. X is the shutter number. Payload can be 0 = stop, 1 = up, 2 = down. Repeating UP/DOWN also stops the shutters.
    - <code>/cmnd/tX</code>: timed channel control. X is the timed channel number. Payload can be 0 = off, 1 = on, 2 = toggle.
    Light, simple, shutter and timed channel numbers are different from device channel numbers. For example if you set up 6 light channels, you can assign them to device channels 6, 8, 10, 12, 14 and 16. Shutters take up 2 device channels each (1 for up movement and 1 for down movement).

- channel status topic: <code>/channel/X</code> where X is the device channel number. Payload will be 0 for OFF and 1 for ON. It is updated by the ESP after processing commands, statuses are sent with retain flag.
- shutter status topic: <code>/shutter/X</code> where X is the shutter number. It holds the current shutter positions. Payload will be 0 for unknown/partly open, 1 for UP, 2 for DOWN.

- status topic: <code>/stat</code> where the ESP sends periodic updates (heartbeats) with useful information.
- will topic: <code>/will</code> Payload is 1 when device is ON and 0 when powered OFF

# Libraries
- https://github.com/256dpi/arduino-mqtt
- https://github.com/bblanchon/ArduinoJson

# PCB example
<img src="https://github.com/cctweaker/esplights/blob/master/Hardware/ESPlights v1.0 top example.jpg?raw=true">
<img src="https://github.com/cctweaker/esplights/blob/master/Hardware/ESPlights v1.0 bottom example.jpg?raw=true">