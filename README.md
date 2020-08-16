# ESP32 Nextion Display Driver

A simple and small ESP32 driver for any [Nextion](https://nextion.tech/) HMI display.  
Developed in **C** using the [esp-idf](https://github.com/espressif/esp-idf) framework.  

Everything was based on Nextion [instruction set](https://nextion.tech/instruction-set/).  

## Capabilities

* Send commands
* Listen to device events
* Configuration via [menuconfig](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html) _(Kconfig)_

## Installation Instructions

### Using esp-idf

1. Clone or download and extract the repository to the components folder of your ESP-IDF project
2. Configure it in `menuconfig -> Component config -> Nextion Display` _(optional)_
3. Include `nextion.h` in your code

### Using PlatformIO ⚠️pending⚠️

On the `env` section of `platformio.ini`, add the following:

```ini
[env]
lib_deps =
  esp32-nextion
```

## To Do

* [ ] Create tests
* [ ] Helper functions covering all methods
* [ ] Macro helpers for command creation
* [ ] Publish on PlatformIO
* [ ] Discover how to handle touch events with coordinates
