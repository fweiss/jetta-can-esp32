# jetta-can-esp32
This project uses CAN bus

## Hardware
Connect the following via the wiring diagram:

- Sparkfun This ESP32
- Microchip MCP2551

Connections are:
MCP2551 -- ESP32 Thing

- VSS -- GND
- VDD (5V) -- VUSB
- TXD -- 21 (GPIO21)
- RXD -- 22 (GPIO22)

## MQTT
Schema idea:

/jetta-can/status/engine/rpm: 2200
