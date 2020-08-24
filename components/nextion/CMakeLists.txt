
idf_component_register(
    SRCS
        "nextion.c"
        "nextion_parse.c"
        "components/rb-srsw/components/rb-srsw/ringbuffer.c"
    INCLUDE_DIRS
        "include"
        "components/rb-srsw/components/rb-srsw/include"
    PRIV_INCLUDE_DIRS
        "private_include"
        "components/rb-srsw/components/rb-srsw/private_include"
    REQUIRES
        freertos
        driver
)