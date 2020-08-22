idf_component_register(
    SRCS "nextion.c" "nextion_parse.c" "ringbuffer.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "private_include"
    REQUIRES freertos driver
)