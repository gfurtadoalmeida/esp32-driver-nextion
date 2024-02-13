# nextion.h

## Functions

* ```nextion_driver_install```: Installs the Nextion driver and creates a Nextion context with the driver.
* ```nextion_driver_delete```: Deletes a Nextion driver and context.
* ```nextion_init```: Does the initializations necessary before any action can be done.
* ```nextion_command_send```: Sends a command that waits for a simple response (ACK).
* ```nextion_command_send_variadic```: Sends a command that waits for a simple response (ACK). Variadic version.
* ```nextion_command_send_get_bytes```: Sends a command that returns bytes.
* ```nextion_event_callback_set_on_touch```: Sets a callback for when a component is touched; 'on touch' events.
* ```nextion_event_callback_set_on_touch_coord```: Sets a callback for when something is touched and "sendxy=1"; 'on touch with coordinates' events.
* ```nextion_event_callback_set_on_device```: Sets a callback for when a device event happens; 'on device' events.
* ```nextion_transparent_data_mode_begin```: Begins the "Transparent Data Mode".
* ```nextion_transparent_data_mode_write```: Writes a value onto the device serial buffer.
* ```nextion_transparent_data_mode_end```: Ends the "Transparent Data Mode".