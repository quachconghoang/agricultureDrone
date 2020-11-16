cmake_minimum_required(VERSION 3.5)

project(IoT-Dashboard LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Mqtt REQUIRED)
find_package(libbsoncxx REQUIRED)
find_package(libmongocxx REQUIRED)

add_executable(IoT-Dashboard
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  resources.qrc
  ToggleButton.cpp
  ToggleButton.h
        SensorsMQTT.cpp SensorsMQTT.h)

include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/include/libbson-1.0")

target_link_libraries(IoT-Dashboard PRIVATE Qt5::Widgets Qt5::Mqtt)
target_link_libraries(IoT-Dashboard PRIVATE ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(IoT-Dashboard PRIVATE ${LIBBSONCXX_LIBRARIES})
