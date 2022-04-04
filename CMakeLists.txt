cmake_minimum_required(VERSION 3.20)
project(Lab2Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Lab3Project main.cpp mainwindow.cpp mainwindow.h paint_widget.cpp paint_widget.h)
target_link_libraries(Lab3Project
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )