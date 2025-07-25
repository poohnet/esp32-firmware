ESP32 Firmware
==============

This repository contains the firmware source code for all Tinkerforge
products that are based on the ESP32 Brick or ESP32 Ethernet Brick.
This includes WARP Chargers and WARP Energy Managers of any generation.

Repository Overview
-------------------

.. DO NOT EDIT THIS OVERVIEW MANUALLY! CHANGE https://github.com/Tinkerforge/esp32-firmware/repo_overview.rst AND COPY THAT BLOCK INTO ALL REPOS LISTED BELOW. TODO: AUTOMATE THIS

Software
~~~~~~~~
- `esp32-firmware <https://github.com/Tinkerforge/esp32-firmware>`__  **Please report any issues concerning WARP hard- and software here!** Source code of the ESP32 firmware shared between all WARP Chargers and Energy Managers

- `tfjson <https://github.com/Tinkerforge/tfjson>`__ SAX style JSON serializer and deserializer
- `tfmodbustcp <https://github.com/Tinkerforge/tfmodbustcp>`__ Modbus TCP server and client implementation
- `tfocpp <https://github.com/Tinkerforge/tfocpp>`__ OCPP 1.6 implementation
- `tftools <https://github.com/Tinkerforge/tftools>`__ Miscellaneous tools and helpers

- `esp32-remote-access <https://github.com/Tinkerforge/esp32-remote-access>`__ Source code of the my.warp-charger.com remote access server

- `warp-charger <https://github.com/Tinkerforge/warp-charger>`__ The source code of (docs.)warp-charger.com and the printed manual, released firmwares, datasheets and documents, as well as some tools and hardware design files
- `api.warp-charger.com <https://github.com/Tinkerforge/api.warp-charger.com>`__ Serves APIs that are used by WARP Chargers to obtain relevant public information like day ahead prices
- `vislog.warp-charger.com <https://github.com/Tinkerforge/vislog.warp-charger.com>`__ Visualizes WARP Charger logs and EVSE debug protocols
- `dbus-warp-charger <https://github.com/Tinkerforge/dbus-warp-charger>`__ Integrates WARP Chargers into a Victron Energy Venus OS device (e.g. Cerbo GX)

WARP Charger Hardware
~~~~~~~~~~~~~~~~~~~~~~

- `esp32-brick <https://github.com/Tinkerforge/esp32-brick>`__ Hardware design files of the ESP32 Brick
- `evse-bricklet <https://github.com/Tinkerforge/evse-bricklet>`__  Firmware source code and hardware design files of the EVSE Bricklet
- `rs485-bricklet <https://github.com/Tinkerforge/rs485-bricklet>`__ Firmware source code and hardware design files of the RS485 Bricklet

WARP2 Charger Hardware
~~~~~~~~~~~~~~~~~~~~~~

- `esp32-ethernet-brick <https://github.com/Tinkerforge/esp32-ethernet-brick>`__ Hardware design files of the ESP32 Ethernet Brick
- `evse-v2-bricklet <https://github.com/Tinkerforge/evse-v2-bricklet>`__ Firmware source code and hardware design files of the EVSE 2.0 Bricklet
- `nfc-bricklet <https://github.com/Tinkerforge/nfc-bricklet>`__ Firmware source code and hardware design files of the NFC Bricklet

WARP3 Charger Hardware
~~~~~~~~~~~~~~~~~~~~~~

- `warp-esp32-ethernet-brick <https://github.com/Tinkerforge/warp-esp32-ethernet-brick>`__ Hardware design files of the WARP ESP32 Ethernet Brick
- `evse-v3-bricklet <https://github.com/Tinkerforge/evse-v3-bricklet>`__ Firmware source code and hardware design files of the EVSE 3.0 Bricklet
- `nfc-bricklet <https://github.com/Tinkerforge/nfc-bricklet>`__ Firmware source code and hardware design files of the NFC Bricklet

WARP Energy Manager Hardware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `esp32-ethernet-brick <https://github.com/Tinkerforge/esp32-ethernet-brick>`__ Hardware design files of the ESP32 Ethernet Brick
- `warp-energy-manager-bricklet <https://github.com/Tinkerforge/warp-energy-manager-bricklet>`__ Firmware source code and hardware design files of the WARP Energy Manager Bricklet

WARP Energy Manager 2.0 Hardware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `esp32-ethernet-brick <https://github.com/Tinkerforge/esp32-ethernet-brick>`__ Hardware design files of the ESP32 Ethernet Brick
- `warp-energy-manager-v2-bricklet <https://github.com/Tinkerforge/warp-energy-manager-v2-bricklet>`__ Firmware source code and hardware design files of the WARP Energy Manager 2.0 Bricklet
- `warp-front-panel-bricklet <https://github.com/Tinkerforge/warp-front-panel-bricklet>`__ Firmware source code and hardware design files of the WARP Front Panel Bricklet

Forked/patched projects
~~~~~~~~~~~~~~~~~~~~~~~

- `arduino-esp32 <https://github.com/Tinkerforge/arduino-esp32>`__
- `esp32-arduino-libs <https://github.com/Tinkerforge/esp32-arduino-libs>`__
- `WireGuard-ESP32-Arduino <https://github.com/Tinkerforge/WireGuard-ESP32-Arduino>`__


Repository Content
------------------

provisioning/:
 * Scripts for mass provisioning of ESP32 modules

software/:
 * build/: Compiled files will be put here
 * src/: Source of the firmware, including modules
 * web/: Source of the web interface, including modules

Software
--------

See software/README.txt for build instructions.
More documentation of how the software and the build process work
will follow in the future. For now in brief:

* The software is built with https://platformio.org/
* For each variation of the firmware (warp, warp2, esp32, esp32_ethernet, ...)
  there is a corresponding *.ini file specifying the PlatformIO environment used
  to build that variation. The environments mostly differ in the backend (i.e.
  firmware) and frontend (i.e. web interface) modules selected to be compiled
  into the firmware.
* Custom hooks compile the web interface from TypeScript and Sass into JavaScript
  and CSS, place everything in one HTML file, zip it and create a C header that
  is then compiled in the firmware.
* After the firmware is built in the software/build folder, the custom hooks
  merge the firmware, bootloader, partition table, etc into one bin file that
  can be flashed on the ESP32 Brick at offset 0x1000 or can be uploaded to a
  running WARP Charger.
