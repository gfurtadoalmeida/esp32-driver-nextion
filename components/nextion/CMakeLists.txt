file(GLOB srcsNEXTION "src/*.c")

idf_component_register(
    SRCS
        ${srcsNEXTION}
    INCLUDE_DIRS
        "include"
        "external/iot-lib-nextion/lib/include"
    PRIV_INCLUDE_DIRS
        "private_include"
    REQUIRES
        driver
)

add_subdirectory("external/iot-lib-nextion")

target_link_libraries(${COMPONENT_LIB} PRIVATE iot-lib-nextion)