idf_component_register(
    SRCS "driver/nextion.c"
    INCLUDE_DIRS "driver/include"
    PRIV_INCLUDE_DIRS "driver/private_include"
    REQUIRES freertos driver
)