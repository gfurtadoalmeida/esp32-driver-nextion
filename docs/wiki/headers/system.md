# system.h

Functions to work with the device system.

## Behavior

* ```nextion_system_reset```: reset the display, losing all volatile configuration.
* ```nextion_system_set_send_xy```: set if the display will send x and y touch coordinates on every touch.
* ```nextion_system_sleep```: enter in the sleep mode.
* ```nextion_system_wakeup```: exit the sleep mode.

## Configuration

* ```nextion_system_get_brightness```: get the display brightness.
* ```nextion_system_get_sleep_on_no_serial```: get how long the display will be on after the last serial command.
* ```nextion_system_get_sleep_on_no_touch```: get how long the display will be on after the last touch.
* ```nextion_system_set_brightness```: set the display brightness.
* ```nextion_system_set_sleep_on_no_serial```: set how long the display will be on after the last serial command.
* ```nextion_system_set_sleep_on_no_touch```: set how long the display will be on after the last touch.
* ```nextion_system_set_wake_on_touch```: set if the display will turn on when touched.
* ```nextion_system_set_wake_on_serial```: set if the display will turn when receiving serial data.

## Variable

* ```nextion_system_get_variable_number```: get a signed number from a variable.
* ```nextion_system_get_variable_text```: get a text from a variable.
* ```nextion_system_set_variable_number```: set a variable value with a number.
* ```nextion_system_set_variable_text```: set a variable value with a text.
