cmake_minimum_required(VERSION 3.25)
project(CANTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include(cmake/StandardSettings.cmake)
#include(cmake/StaticAnalyzers.cmake)
#include(cmake/Utils.cmake)
#include(cmake/Vcpkg.cmake)
#include(cmake/Doxygen.cmake)

ADD_LIBRARY(BoatLibrary
        lib/communication/canbus.cpp lib/communication/canbus.h
        lib/configuration/Config.cpp lib/configuration/Config.h
        lib/configuration/ConfigSection.cpp lib/configuration/ConfigSection.h)

add_executable(CANTest main.cpp)
target_link_libraries(${PROJECT_NAME} BoatLibrary spdlog lely-io2 lely-util lely-can lely-ev lely-coapp pthread config++ paho-mqtt3as paho-mqttpp3 fmt)
