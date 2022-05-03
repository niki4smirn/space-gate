cmake_minimum_required(VERSION 3.19)
project(space-gate)

set(CMAKE_CXX_FLAGS"{$CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_PREFIX_PATH "~/Qt/5.15.2/gcc_64/lib/cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        WebSockets
        REQUIRED
        )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Protobuf REQUIRED)
set(scheme_PROTOS
        Protobuf/events.proto
        Protobuf/client_events.proto
        Protobuf/server_events.proto
        )
include_directories(${Protobuf_INCLUDE_DIRS})

set(COMMON_SRC
        src/AbstractController/abstract_controller.cpp
        src/Helpers/helpers.cpp
        Constants/constants.cpp
        )

add_executable(server
        ${COMMON_SRC}
        src/Server/server_main.cpp
        src/Server/Controllers/server_controller.cpp
        src/Server/Models/User/user.cpp
        src/Server/Models/ServerModel/server_model.cpp
        src/Server/Controllers/room_controller.cpp
        src/Server/Models/RoomModel/room_model.cpp
        )

add_executable(client
        ${COMMON_SRC}
        src/Client/client_main.cpp
        src/Client/Controller/client_controller.cpp
        src/Client/background_widget.cpp
        src/Client/background_widget.h
        src/Client/client_main_menu.cpp
        src/Client/client_main_menu.h
        src/Client/client_view.cpp
        src/Client/client_view.h
        src/Client/stars.cpp
        src/Client/stars.h
        )

set(REQUIRED_QT_PARTS
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::WebSockets
        )

foreach (executable server; client)
    target_link_libraries(${executable} ${REQUIRED_QT_PARTS})
    target_link_libraries(${executable} protobuf::libprotobuf)
    protobuf_generate(TARGET ${executable} PROTOC_OUT_DIR ../ PROTOS ${scheme_PROTOS})
endforeach()