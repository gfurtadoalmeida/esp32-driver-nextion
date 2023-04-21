file(GLOB srcsCOMP "src/*.c")

idf_component_register(
    SRCS
        ${srcsCOMP}
    INCLUDE_DIRS
        "include"
    PRIV_INCLUDE_DIRS
        "private_include"
    REQUIRES
        driver
)