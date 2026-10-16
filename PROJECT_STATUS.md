Changelog
v0.3-mqtt-manager
Date: 2026-10-16

Added
MQTTManager class
MQTT reconnect handling
MQTT Last Will support
MQTT system topic publishing
MQTT Topics
weather/status
weather/system/ip
weather/system/rssi
weather/system/heap
weather/system/psram
weather/system/uptime
Refactoring
Moved MQTT logic from main.cpp
Simplified application startup sequence
Added modular MQTT architecture
Fixed
MQTT authentication issues
MQTT reconnect stability
MQTT publish workflow
v0.2-mqtt
Date: 2026-10-16

Added
MQTT broker connectivity
MQTT authentication
MQTT publish support
MQTT Topics
weather/status
weather/system/ip
weather/system/rssi
weather/system/heap
weather/system/psram
v0.1-stable
Date: 2026-10-16

Added
ConfigManager
NVS configuration storage
WiFi AP Mode
WiFi STA Mode
Web Dashboard
Configuration Portal
Device Restart Endpoint
Endpoints
/
/setup
/save
/reboot
