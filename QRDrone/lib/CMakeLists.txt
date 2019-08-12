# Library CMakeLists.txt
cmake_minimum_required (VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)

project (arDroneLibraryProject)
find_package( OpenCV REQUIRED )

set(PARH_TO_ARDRONE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cvdrone/src/ardrone)

add_library(arDroneLibrary
    ${PARH_TO_ARDRONE_SOURCES}/ardrone.cpp
    ${PARH_TO_ARDRONE_SOURCES}/command.cpp
    ${PARH_TO_ARDRONE_SOURCES}/config.cpp
    ${PARH_TO_ARDRONE_SOURCES}/udp.cpp
    ${PARH_TO_ARDRONE_SOURCES}/tcp.cpp
    ${PARH_TO_ARDRONE_SOURCES}/navdata.cpp
    ${PARH_TO_ARDRONE_SOURCES}/version.cpp
    ${PARH_TO_ARDRONE_SOURCES}/video.cpp
)

target_link_libraries(arDroneLibrary ${OpenCV_LIBS} "-D__STDC_CONSTANT_MACROS -lm -lpthread -lavutil -lavformat -lavcodec -lswscale")


