cmake_minimum_required(VERSION 3.13)

if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(nextion)