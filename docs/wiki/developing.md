# Developing

## Building

### On VS Code

For a better experience it's highly recommended to install the official [ESP-IDF Extension](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-extension).

```ctrl+shif+b``` or ```ctrl+shif+p -> Task: Run Task -> Build```

### With ESP-IDF

Run the ```idf.py build``` command on the root folder.

### With the Provided Script

Run the ```project.ps1 build``` command on the root folder.

## Debugging

In the [.debug](../../.debug) folder you'll find interface configuration files for debug boards. Choose one and copy it to the ```%IDF_TOOLS_PATH%\tools\openocd-esp32\{version}\openocd-esp32\share\openocd\scripts\interface``` folder.

### Boards

* [CJMCU-FTDI-2232HL](https://www.aliexpress.com/w/wholesale-CJMCU%2525252d2232HL.html)

### Driver Configuration

1. Install the latest [FTDI VPC Driver](https://ftdichip.com/drivers/vcp-drivers/).
2. Plug the debug board.
3. Open [Zadig](https://zadig.akeo.ie/) and replace the ```Dual RS232-HS (Interface 0)``` driver with WinUSB (```Options->List all devices```).

## Testing Using the Nextion Simulator

1) Download and install the [simulator](https://nextion.tech/nextion-editor/#_section2).
2) Get an USB to TTL (FT232) device - from now on called `adapter` - so you can connect your ESP32 to the simulator.
3) Connect the `adapter` to your PC.
4) Open the [test_display.hmi](../../components/esp32_driver_nextion/test/hmi/) on the simulator and start the debug.
5) Connect the simulator to the COM port used by the `adapter`.
6) Connect the ESP32 to the `adapter`.
7) Run the tests.

> [!NOTE]
>
> * The display project was built using Nextion Editor 1.67.1; you might need to [upgrade your display](https://nextion.tech/faq-items/using-legacy-nextion-devices/) before uploading the HMI.  
> * The task watchdog for CPU0 is disabled on the test project so we can interact with the testing tool.  
> * The baudrate is set to **115200** on the display ```program.s``` and ESP32 [test_runner.c](../../test/main/test_runner.c).

### On VS Code

1. Build the test project: `ctrl+shif+p -> Task: Run Task -> Build test`
2. Flash the test project: `ctrl+shif+p -> Task: Run Task -> Flash test`
3. Monitor the test run: `ctrl+shif+p -> Task: Run Task -> Monitor test`

You can do it in one command using `ctrl+shif+p -> Task: Run Task -> Build, flash and start a monitor for the tests`

### With ESP-IDF

All commands must be run on the test runner folder.  
Change the COM port to the one you're using.  

1. Build the test project: `idf.py build`
2. Flash the test project: `idf.py flash -p COM3`
3. Monitor the test run: `idf.py monitor -p COM3`
