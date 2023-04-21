# ESP32 Nextion Display Driver

![GitHub Build Status](https://github.com/gfurtadoalmeida/esp32-driver-nextion/actions/workflows/build.yml/badge.svg) [![Bugs](https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=bugs)](https://sonarcloud.io/summary/new_code?id=esp32_driver_nextion) [![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=esp32_driver_nextion) [![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=esp32_driver_nextion) [![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=esp32_driver_nextion&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=esp32_driver_nextion)  
ESP32 driver for [Nextion](https://nextion.tech/) HMI displays. 

## Characteristics

* ESP-IDF: [v4.4.4](https://docs.espressif.com/projects/esp-idf/en/v4.4.4/esp32/index.html)
* Written in **C** using just the [ESP-IDF](https://github.com/espressif/esp-idf) framework.
* Testable: 95+ tests.
* Unopinionated: you get the functions and that's it.
* Callback for events: no `do_events()` loops.
* Multi-display: you can connect to as many displays as you want.
* Configurable: using [menuconfig](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html).

## Documentation

Everything is on the [wiki](https://github.com/gfurtadoalmeida/esp32-driver-nextion/wiki).

## To Do

- [ ] Fix code smells.
- [ ] Implement automatic baud rate detection on `nextion_init`.