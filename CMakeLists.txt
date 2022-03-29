cmake_minimum_required(VERSION 3.19)
project(space-gate)

set(CMAKE_CXX_FLAGS"{$CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_STANDARD 20)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        WebSockets
        REQUIRED
        )

set(COMMON_SRC
        src/AbstractController/abstract_controller.cpp)

add_executable(server
        ${COMMON_SRC}
        src/Server/server_main.cpp
        src/Server/Controller/server_controller.cpp
        src/Server/Models/User/user.cpp
        src/Server/Models/ServerModel/server_model.cpp
        )

add_executable(client
        ${COMMON_SRC}
        src/Client/client_main.cpp
        src/Client/Controller/client_controller.cpp
        )

set(REQUIRED_LIBS
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::WebSockets
        )

target_link_libraries(server ${REQUIRED_LIBS})
target_link_libraries(client ${REQUIRED_LIBS})


