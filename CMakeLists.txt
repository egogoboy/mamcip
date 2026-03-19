cmake_minimum_required(VERSION 3.21)
project(app LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5/lib/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(app
    src/main.cpp
    src/ui.cpp
)

include_directories(app PRIVATE src)

target_link_libraries(app PRIVATE Qt5::Widgets)
