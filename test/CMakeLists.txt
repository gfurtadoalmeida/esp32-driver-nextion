cmake_minimum_required(VERSION 3.13)

if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

set(EXTRA_COMPONENT_DIRS "../components")

set(TEST_COMPONENTS "nextion" CACHE STRING "List of components to test")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(test_runner)
