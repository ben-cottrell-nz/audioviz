cmake_minimum_required(VERSION 3.18)
project(audioviz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_PREFIX_PATH /opt/6.2-linux-g++-static/lib/cmake)
endif()
find_package(Qt6
        COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(RTAUDIO_PATH /home/ben/Documents/repos/rtaudio)
elseif (WIN32)
    set(RTAUDIO_PATH "C:/Users/ben/Downloads/rtaudio-5.1.0")
    add_link_options(-pthread -static-libgcc -static-libstdc++ -mwindows)
endif ()
include_directories(${RTAUDIO_PATH})
link_directories(${RTAUDIO_PATH}/build)
add_executable(audioviz
        main.cpp
        mainwindow.cpp
        plotwidget.cpp
        audioengine.cpp
        resources.qrc
        settingsdialog.cpp
        settingsdialog.h
        settingsdialog.ui
        aboutdialog.cpp
        aboutdialog.h
        aboutdialog.ui configmanager.cpp configmanager.h)
add_executable(test test.cpp)
target_link_libraries(test rtaudio)
target_link_libraries(audioviz
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        rtaudio
        fftw3
        )
if (WIN32)
    target_link_libraries(audioviz
            pthread)
endif ()


