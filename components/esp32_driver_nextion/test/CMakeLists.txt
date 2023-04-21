file(GLOB srcsCOMP "*.c")

idf_component_register(
    SRCS
        ${srcsCOMP}
    INCLUDE_DIRS
        "../include"
    PRIV_INCLUDE_DIRS
        "../private_include"
        "include"
    REQUIRES
        unity
        esp32_driver_nextion
)
