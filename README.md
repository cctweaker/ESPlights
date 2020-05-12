# ESPlights

MQTT controlling software for the beta version of 16 channel lights controller provided by https://3dstar.ro

# Features
- secure MQTT connection, Let's Ecrypt CA cetificate already loaded
- topic structure for building control: location/device/+
- easy integration with Home Assistant (not yet automatic)
- lights state saved and recovered on restart
- on / off / toggle command for each channel
- separate status topic for each channel
- main and backup WiFi connections
- secured, no access point is presented in case both WiFi connections are unavailable
- no web access, just pure MQTT

# To do
- automatic Home Assistant integration
- configure topic, configure new devices by phone/app