# Using the Component

## Project Configuration

### 1. Clone the Repository

Clone this repository and copy the folder ["components/esp32_driver_nextion"](../../components/esp32_driver_nextion/) to the components folder of your ESP-IDF project.

### 2. Configure

Configure it in ```menuconfig -> Component config -> Nextion Display``` (optional).

### 3. Add the Headers

Include `esp32_driver_nextion/nextion.h` in your code.

## Display Configuration

## 1. Set the Baud Rate :warning:

Set the baud rate on the device ```program.s``` using either ```baud``` or ```bauds``` command; this library does not have automatic baud rate detection.

## 2. Configure for Touch Event (optional)

If you want to receive a callback when a component is touched (using `nextion_event_callback_set_on_touch`) you must set the `Send Component ID` checkbox in the display editor, on the component you want to get the event called for.
