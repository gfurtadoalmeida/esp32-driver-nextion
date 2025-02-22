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

## 2. Listen to Events (optional)

Events are fired using the `esp_event` infrastructure, so if you want to listen to them you must:

1) Have an event loop configured (usually calling `esp_event_loop_create_default` at start is enough).
2) Register an callback using `esp_event_handler_register`.

The event base and events id are in the [events.h](../../components/esp32_driver_nextion/include/esp32_driver_nextion/base/events.h) header:

* Event base: `NEXTION_EVENT`.
* Events id:
  * `NEXTION_EVENT_TOUCHED`: touch event without coordinates.
  * `NEXTION_EVENT_TOUCHED_COORD`: touch with coordinates.
  * `NEXTION_EVENT_STATE_CHANGED`: device state changed.

For touch events you must the `Send Component ID` checkbox in the display editor, on the component you want to get the event called for.
