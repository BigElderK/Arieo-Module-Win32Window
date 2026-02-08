cmake_minimum_required (VERSION 3.31)

include($ENV{ARIEO_BUILDENV_PACKAGE_INSTALL_FOLDER}/cmake/build_environment.cmake)
project(ArieoEngine)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    arieo_engine_project(
        arieo_win32_window_module
        ALIAS arieo::modules::window::win32
        PROJECT_TYPE module
        SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/private/src/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/private/src/*/*.cpp
        INTERFACES
            Arieo-Interface-Window::arieo_window_interface
        PACKAGES
            Arieo-Interface-Window
            Arieo-Core
        PRIVATE_LIBS
            arieo::base
            arieo::core
    )
endif()