file(GLOB srcsTstNEXTION "nextion/*.c")

idf_component_register(
    SRCS
        ${srcsTstNEXTION}
    INCLUDE_DIRS
        "../include"
        "."
    PRIV_INCLUDE_DIRS
        "../private_include"
    REQUIRES
        unity
        nextion
)
