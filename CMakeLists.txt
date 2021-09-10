cmake_minimum_required(VERSION 3.20)
project(audioviz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        REQUIRED)
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	set(RTAUDIO_PATH /home/ben/Documents/repos/rtaudio)
elseif (WIN32)
	set(RTAUDIO_PATH "C:/Users/ben/Downloads/rtaudio-5.1.0")
endif()
include_directories(Qt5::Multimedia ${RTAUDIO_PATH})
link_directories(${RTAUDIO_PATH}/build)
add_executable(audioviz
        main.cpp
        mainwindow.cpp
        plotwidget.cpp
        audioengine.cpp)

add_executable(test test.cpp)
target_link_libraries(test rtaudio)
target_link_libraries(audioviz
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Multimedia
        rtaudio
        )
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	target_link_libraries(audioviz
			fftw3)
endif()

