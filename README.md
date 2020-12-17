# ESP32 Nextion Display Driver

![Azure DevOps builds](https://img.shields.io/azure-devops/build/gfurtadoalmeida/GitHub/39?)  
ESP32 driver for [Nextion](https://nextion.tech/) HMI displays.  

## Characteristics

* Written in **C** using the [ESP-IDF](https://github.com/espressif/esp-idf) framework.
* Unopinionated: you get the functions and that's it.
* Multi-display: you can connect to as many displays as you want.
* Based on Nextion [instruction set](https://nextion.tech/instruction-set/).
* Configurable using [menuconfig](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html) _(Kconfig)_.

## Installation Instructions

### Using ESP-IDF

1. Clone this repository using `git clone --recursive` and copy the folder ["components/nextion"](/components/nextion/) to the components folder of your ESP-IDF project.
2. Configure it in `menuconfig -> Component config -> Nextion Display` _(optional)_.
3. Include `nextion-esp32.h` in your code.

## Project Structure

```text
/
|-- components
    |-- nextion             : The driver
        | external          : External libraries
        |-- include         : Includes that can be used by dependent projects
        |-- private_include : Includes that only the driver can use
        |-- src             : Where ".c" files resides
        |-- test            : Tests
            | -- nextion
                 |-- hmi    : Nextion display project used for testing
|-- examples                : Some usage examples
|-- main                    : Where debugging happens
|-- test                    : Test runner
```

## Development Guidelines

* The code must run on any ESP32+ hardware.
* Create tests for every new functionality.

## Dependencies

* [iot-lib-nextion](https://github.com/gfurtadoalmeida/iot-lib-nextion): portable Nextion library.

## Build Requirements

* [CMake](https://cmake.org/): 3.5+, must be on PATH environment variable.
* [ESP-IDF](https://github.com/espressif/esp-idf): use the the last stable one.

## Building

### On VS Code

It's highly recommended to install the official [ESP-IDF Extension](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-extension), for a better experience.

```ctrl+shif+b``` or ```ctrl+shif+p -> Task: Run Task -> Build```

### With ESP-IDF

```idf.py build``` on the root folder.

## Testing

Upload [test_display.hmi](/components/nextion/test/nextion/hmi/) on your Nextion display before testing. Be aware that the display project was built using Nextion Editor 1.61.2; you might need to [upgrade your display](https://nextion.tech/faq-items/using-legacy-nextion-devices/) before uploading this project.  

The task watchdog for CPU0 is disabled on the test project. It is needed so we can interact with the test tool.  

The baudrate is set to 115200 on the display `program.s` and ESP32 [common_infra_test.h](/components/nextion/test/common_infra_test.h)

### On VS Code

1. Build the test project: `ctrl+shif+p -> Task: Run Task -> Build test`
2. Flash the test project: `ctrl+shif+p -> Task: Run Task -> Flash test`
3. Monitor the test run: `ctrl+shif+p -> Task: Run Task -> Monitor test`

You can do it in one command using `ctrl+shif+p -> Task: Run Task -> Build, flash and start a monitor for the tests`

### With ESP-IDF

All commands must be run on the test runner folder.  
Change the COM port to the one you're using.

1. Build the test project: `idf.py build`
2. Flash the test project: `idf.py flash -p COM4`
3. Monitor the test run: `idf.py monitor -p COM4`

## To Do

* [ ] Documentation
