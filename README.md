# ESP32 Nextion Display Driver

A simple and small ESP32 driver for any [Nextion](https://nextion.tech/) HMI display.  
Written in **C** using the [ESP-IDF](https://github.com/espressif/esp-idf) framework.  

Everything is based on Nextion [instruction set](https://nextion.tech/instruction-set/).  

## Capabilities

* Send commands.
* Listen to device events.
* Configuration via [menuconfig](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html) _(Kconfig)_.

## Installation Instructions

### Using ESP-IDF

1. Clone or download this repository and copy the folder ["components/nextion"](/components/nextion/) to the components folder of your ESP-IDF project.
2. Configure it in `menuconfig -> Component config -> Nextion Display` _(optional)_.
3. Include `nextion.h` in your code.

### Using PlatformIO _(pending)_ ⚠️

On the `env` section of `platformio.ini`, add the following:

```ini
[env]
lib_deps =
  esp32-nextion
```

## Project Structure

```text
/esp32-driver-nextion
|-- components
    |-- nextion             : The driver
        |-- include         : Includes that can be used by dependent projects
        |-- private_include : Includes that only the driver can use
        |-- test            : Tests! 💪
            |-- hmi         : Nextion display project used for testing
|-- examples                : Some usage examples
|-- main                    : Where debugging happens
|-- test                    : Test runner
```

## Building

### On VS Code

```ctrl+shif+b``` or ```ctrl+shif+p -> Task: Run Task -> Build```

### With ESP-IDF

```idf.py build``` on the root folder.

## Testing

Upload [test_display.hmi](/components/nextion/test/hmi/) on your Nextion display before testing. Be aware that the display project was built using Nextion Editor 1.16.1; you might need to [upgrade your display](https://nextion.tech/faq-items/using-legacy-nextion-devices/) before uploading this project.  

The task watchdog for CPU0 is disabled on the test project. It is needed so we can interact with the test tool.  

For your sanity sake, power your display with a decent power source (5V +700 mA), otherwise you'll waste 4+ hours debugging just to discover that the power source was the culprit. 🤦‍♂️

### On VS Code

1. Build the test project: `ctrl+shif+p -> Task: Run Task -> Build test`
2. Flash the test project: `ctrl+shif+p -> Task: Run Task -> Flash test`
3. Monitor the test run: `ctrl+shif+p -> Task: Run Task -> Monitor test`

### With ESP-IDF

All commands must be run on the test runner folder.

1. Build the test project: `idf.py build`
2. Flash the test project: `idf.py flash -p COM4`
3. Monitor the test run: `idf.py monitor -p COM4`

## To Do

* [ ] Helper functions covering all methods
* [ ] Macro helpers for command creation
* [ ] Discover how to handle touch events with coordinates
* [ ] Publish on PlatformIO
