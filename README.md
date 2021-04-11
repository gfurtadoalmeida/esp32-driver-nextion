# ESP32 Nextion Display Driver

![Azure DevOps builds](https://img.shields.io/azure-devops/build/gfurtadoalmeida/GitHub/39?)  
ESP32 driver for [Nextion](https://nextion.tech/) HMI displays.  

## Characteristics

* Written in **C** using the [ESP-IDF](https://github.com/espressif/esp-idf) framework and the Nextion [instruction set](https://nextion.tech/instruction-set/).
* Testable: 95+ tests.
* Unopinionated: you get the functions and that's it.
* Callback for events: no `do_events()` loops.
* Multi-display: you can connect to as many displays as you want.
* Configurable: using [menuconfig](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html) _(Kconfig)_.
* Newlib nano: supports the [Newlib](https://sourceware.org/newlib/README) `--enable-newlib-nano-formatted-io` flag (`CONFIG_NEWLIB_NANO_FORMAT` on `sdkconfig`)

## Documentation

Everything is on the [wiki](https://github.com/gfurtadoalmeida/esp32-driver-nextion/wiki).
