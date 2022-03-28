cmake_minimum_required(VERSION 3.19)
project(space-gate)

set(CMAKE_CXX_FLAGS"{$CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_STANDARD 20)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(server main.cpp)

target_link_libraries(server
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )


