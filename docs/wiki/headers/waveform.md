# waveform.h

Functions to work with UI waveforms.

## Behavior

* ```nextion_waveform_start_refesh```: start the default waveform refreshing (refresh on data point add).
* ```nextion_waveform_stop_refesh```: stop the default waveform refreshing (will not refresh when data point added).

## Value

* ```nextion_waveform_add_value```: add a single value to a waveform channel.
* ```nextion_waveform_clear```: clear all waveform channels.
* ```nextion_waveform_clear_channel```: clear a single waveform channel.

## Stream

* ```nextion_waveform_stream_begin```: begin the waveform data streaming.
* ```nextion_waveform_stream_write```: write a value onto the waveform stream.
