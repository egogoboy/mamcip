cmake_minimum_required(VERSION 3.20)
project(app LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(app
    WIN32
    src/main.cpp
    src/ui.cpp
)

target_link_libraries(app PRIVATE Qt5::Widgets)
