file(GLOB srcsNEXTION "src/nextion/*.c")
file(GLOB srcsRGB "src/rgb565/*.c")

idf_component_register(
    SRCS
        ${srcsNEXTION}
        ${srcsRGB}
    INCLUDE_DIRS
        "include"
    PRIV_INCLUDE_DIRS
        "private_include"
    REQUIRES
        driver
)