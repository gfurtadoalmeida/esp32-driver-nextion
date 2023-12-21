# Contributing

* Branch from the head of the default branch.
* Follow the [ESP-IDF code conventions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/contribute/style-guide.html).
* The code must support any [ESP32 module](https://www.espressif.com/en/products/modules).
* Use portable types:
  * ```stdbool.h```: [bool](https://en.wikipedia.org/wiki/C_data_types#Boolean_type).
  * ```stdint.h```: [fixed-width integer](https://en.wikipedia.org/wiki/C_data_types#stdint.h).
  * ```stddef.h```: [pointers and sizes](https://en.wikipedia.org/wiki/C_data_types#Size_and_pointer_difference_types).
* Do not hide pointers behind typedefs.
* Create tests for every new functionality.
* Examples:
  * Must have an HMI file.
  * HMI must use the basic display version unless an advanced feature is needed.