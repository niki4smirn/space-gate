cmake_minimum_required(VERSION 3.20)
project(SpaceGate)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(SpaceGate main.cpp mainwindow.cpp mainwindow.h paint_widget.cpp paint_widget.h stars.cpp stars.h)
target_link_libraries(SpaceGate
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )