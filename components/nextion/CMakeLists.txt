file(GLOB srcsNEXTION "src/nextion/*.c")
file(GLOB srcsRGB "src/rgb565/*.c")
file(GLOB srcsRING "src/ringbuffer/*.c")

idf_component_register(
    SRCS
        ${srcsNEXTION}
        ${srcsRGB}
        ${srcsRING}
    # Only add the "include" folder; users must choose the folder they want to import from.
    INCLUDE_DIRS
        "include"
    # Only add the "private_include" folder; developers must choose the folder they want to import from.
    PRIV_INCLUDE_DIRS
        "private_include"
    REQUIRES
        freertos
        driver
)