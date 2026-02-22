cmake_minimum_required (VERSION 3.31)

include($ENV{ARIEO_BUILD_ENV_PACKAGES_OUTPUT_DIR}/cmake/build_environment.cmake)

ARIEO_PACKAGE(
    Arieo-Module-Win32Window
    CATEGORY 02_engine
    DEPENDS
        https://github.com/BigElderK/Arieo-Core.git@main
        https://github.com/BigElderK/Arieo-Interface-Window.git@main
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    ARIEO_ENGINE_PROJECT(
        arieo_win32_window_module
        PROJECT_TYPE module

        DEPENDENCIES
            ARIEO_PACKAGES
                Arieo-Core
                Arieo-Interface-Window
            PRIVATE_LIBS
                Arieo-Interface-Window::arieo_window_interface
                Arieo-Core::arieo_core

        SOURCES
            CXX_SOURCES
                ${CMAKE_CURRENT_LIST_DIR}/private/src/*.cpp
                ${CMAKE_CURRENT_LIST_DIR}/private/src/*/*.cpp
    )
else()
    # Create a dummy install target for non-Windows platforms
    message(STATUS "Arieo-Module-Win32Window: Skipping build for ${CMAKE_SYSTEM_NAME} (Windows only)")
    install(CODE "message(STATUS \"Arieo-Module-Win32Window: No installation needed for ${CMAKE_SYSTEM_NAME}\")")
endif()
