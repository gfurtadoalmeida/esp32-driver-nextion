# system.h

## Functions

* ```nextion_system_get_text```: Sends a command that retrieves a null-terminated string.
* ```nextion_system_get_number```: Sends a command that retrieves a signed number.
* ```nextion_system_reset```: Resets the display, losing all volatile configurations.
* ```nextion_system_sleep```: Enters in sleep mode.
* ```nextion_system_wakeup```: Exits sleep mode.
* ```nextion_system_get_brightness```: Get the display brightness.
* ```nextion_system_set_brightness```: Set the display brightness.
* ```nextion_system_get_sleep_no_touch```: Get how long the display will be on after the last touch.
* ```nextion_system_set_sleep_no_touch```: Set how long the display will be on after the last touch.
* ```nextion_system_get_sleep_no_serial```: Get how long the display will be on after the last serial command.
* ```nextion_system_set_sleep_no_serial```: Set how long the display will be on after the last serial command.
* ```nextion_system_set_wake_on_touch```: Set if the display will turn on when touched.
* ```nextion_system_set_wake_on_serial```: Set if the display will turn when receiving serial data.
* ```nextion_system_set_send_xy```: Set if the display will send x and y touch coordinates on every touch.
