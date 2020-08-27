file(GLOB srcsNEXTION "nextion/*.c")
file(GLOB srcsRGB "rgb565/*.c")
file(GLOB srcsRING "ringbuffer/*.c")
file(GLOB srcsINFRA "infra/*.c")

idf_component_register(
    SRCS
        ${srcsNEXTION}
        ${srcsRGB}
        ${srcsRING}
        ${srcsINFRA}
    INCLUDE_DIRS
        "../include"
    PRIV_INCLUDE_DIRS
        "../private_include"
        "nextion/infra"
    REQUIRES
        unity
        nextion
)
