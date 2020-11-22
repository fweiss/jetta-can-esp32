# jetta-can-esp32
This project uses the ESP32's build-in CAN bus peripheral to control a Jetta instrument cluster.

A previous project used an Arduino Uno and CAN bus shield.
This project is simpler because it only needs an external CAN transceiver chip.

## Hardware
Connect the following via the wiring diagram:

- Sparkfun ESP32 Thing
- CJMCU-2551 Microchip MCP2551 breakout board

Connections are:
MCP2551 -- ESP32 Thing

- VSS -- GND
- VDD (5V) -- VUSB
- TXD -- 21 (GPIO21)
- RXD -- 22 (GPIO22)

## MQTT
Schema idea:

/jetta-can/status/engine/rpm: 2200
