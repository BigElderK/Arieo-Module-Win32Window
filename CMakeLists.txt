cmake_minimum_required (VERSION 3.31)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
arieo_engine_project(
    arieo_win32_window_module
    ALIAS arieo::modules::window::win32
    PROJECT_TYPE module
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/private/src/*/*.cpp
    INTERFACES
        arieo::interface::window
    LIBS
        arieo::base
        arieo::core
)
endif()