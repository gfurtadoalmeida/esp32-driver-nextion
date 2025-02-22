# component.h

Functions to work with UI components like buttons, text fields, etc.

## Behavior

* ```nextion_component_refresh```: refresh a component, bringing it to front.
* ```nextion_component_set_touchable```: set if a component can respond to touch events or not.
* ```nextion_component_set_touchable_all```: set if all components can respond to touch events or not.
* ```nextion_component_set_visibility```: set a component visibility.
* ```nextion_component_set_visibility_all```: set all components visibility.

## Property

* ```nextion_component_get_property_number```: get a number from a component property.
* ```nextion_component_get_property_text```: get a text from a component property.
* ```nextion_component_set_property_number```: set a component property with a number.
* ```nextion_component_set_property_text```: set a component property with a text.

## Value

* ```nextion_component_get_boolean```: get a component `.val` value converted to boolean.
* ```nextion_component_get_text```: get a component `.txt` value.
* ```nextion_component_get_value```: get a component `.val` value.
* ```nextion_component_set_boolean```: set a component `.val` value, converted as boolean.
* ```nextion_component_set_text```: set a component `.txt` value.
* ```nextion_component_set_value```: set a component `.val` value.
