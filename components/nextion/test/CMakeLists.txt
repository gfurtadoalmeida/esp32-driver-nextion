file(GLOB srcsNEXTION "nextion/*.c")
file(GLOB srcsRGB "rgb565/*.c")
file(GLOB srcsRING "ringbuffer/*.c")

idf_component_register(
    SRCS
        ${srcsNEXTION}
        ${srcsRGB}
        ${srcsRING}
    INCLUDE_DIRS
        "../include"
    PRIV_INCLUDE_DIRS
        "../private_include"
    REQUIRES
        unity
        nextion
)
