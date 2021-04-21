file(GLOB srcsTstNEXTION "nextion/*.c")
file(GLOB srcsTstRGB "rgb565/*.c")

idf_component_register(
    SRCS
        ${srcsTstNEXTION}
        ${srcsTstRGB}
    INCLUDE_DIRS
        "../include"
        "."
    PRIV_INCLUDE_DIRS
        "../private_include"
    REQUIRES
        unity
        nextion
)
