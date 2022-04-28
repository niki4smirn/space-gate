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

add_executable(SpaceGate
        src/Client/client_main_menu.h
        src/Client/client_main_menu.cpp
        src/Client/client_view.cpp
        src/Client/client_view.h
        src/Client/client_main.cpp
        src/Client/background_widget.cpp
        src/Client/background_widget.h
        src/Client/stars.cpp
        src/Client/stars.h)

target_link_libraries(SpaceGate
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
