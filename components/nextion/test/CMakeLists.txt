idf_component_register(
    SRCS "nextion_test.c" "nextion_buffer_test.c"
    INCLUDE_DIRS "../include"
    PRIV_INCLUDE_DIRS "../private_include"
    REQUIRES unity nextion
)