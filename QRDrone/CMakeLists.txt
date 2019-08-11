cmake_minimum_required (VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE ON)

project (CMake_calling_existing_make)

find_package( OpenCV REQUIRED )

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/)

add_executable(QRDrone ${CMAKE_SOURCE_DIR}/src/QRDrone.cpp)

add_dependencies(QRDrone arDroneLibrary)

target_link_libraries(QRDrone ${CMAKE_SOURCE_DIR}/lib/cvdrone/src/ardrone/ardrone.h arDroneLibrary ${OpenCV_LIBS} "-D__STDC_CONSTANT_MACROS -lm -lpthread -lavutil -lavformat -lavcodec -lswscale -lzbar")



