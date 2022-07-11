cmake_minimum_required(VERSION 3.19)
project(space-gate)

set(CMAKE_CXX_FLAGS"{$CMAKE_CXX_FLAGS} -Wall")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

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

include_directories(Resources/Images/Buttons)
include_directories(Resources/Images/Bulbs)
include_directories(Resources/Images/Background)
include_directories(Resources/Images/HoleRepair)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Protobuf REQUIRED)
set(scheme_PROTOS
        Protobuf/events.proto
        Protobuf/client_events.proto
        Protobuf/server_events.proto
        Protobuf/minigame_actions.proto
        Protobuf/minigame_responses.proto
        )
include_directories(${Protobuf_INCLUDE_DIRS})

set(COMMON_SRC
        src/AbstractController/abstract_controller.cpp
        src/Helpers/helpers.cpp
        src/Helpers/Constants/constants.cpp
        src/Helpers/logging.cpp
        src/Client/InputController/input_names.cpp
        )

add_executable(server
        ${COMMON_SRC}
        src/Server/server_main.cpp
        src/Server/Controllers/ServerController/server_controller.cpp
        src/Server/Models/User/user.cpp
        src/Server/Models/ServerModel/server_model.cpp
        src/Server/Controllers/RoomController/room_controller.cpp
        src/Server/Controllers/GameController/game_controller.cpp
        src/Server/Models/RoomModel/room_model.cpp
        src/Server/Models/GameModel/game_model.cpp
        src/Server/Games/AbstractMinigame/abstract_minigame.cpp
        src/Server/Games/TerminalMinigame/terminal_minigame.cpp
        src/Server/Games/HoleRepairMinigame/hole_repair.cpp
        src/Helpers/Constants/minigames_settings.cpp
        )

if (WIN32)
    set(SYSTEM_DEPENDENT_SOURCES
            src/Client/InputController/input_names_windows.cpp)
else()
    set(SYSTEM_DEPENDENT_SOURCES
            src/Client/InputController/input_names_unix.cpp)
endif (WIN32)

add_executable(client
        ${COMMON_SRC}
        src/Client/client_main.cpp
        src/Client/Controller/client_controller.cpp
        src/Client/View/MainMenu/Background/background_widget.cpp
        src/Client/View/MainMenu/client_main_menu.cpp
        src/Client/View/client_view.cpp
        src/Client/View/MainMenu/Background/star.cpp
        src/Client/InputController/input_controller.cpp
        ${SYSTEM_DEPENDENT_SOURCES}
        src/Client/View/GameView/game_view.cpp
        Resources/Images/images.qrc
        Resources/Fonts/fonts.qrc
        src/Client/View/GameView/game_view.cpp
        src/Client/View/FinalScreen/final_screen.cpp
        src/Client/View/NetworkProblem/network_problem.cpp
        src/Client/Games/TerminalMinigameView/terminal_minigame_view.cpp
        src/Client/Games/HoleRepairMinigameView/hole_repair_view.cpp
        src/Helpers/Constants/minigames_settings.cpp
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

    protobuf_generate(TARGET ${executable}  PROTOC_OUT_DIR  ../ PROTOS  ${scheme_PROTOS})


endforeach()

