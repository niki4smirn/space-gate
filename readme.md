# SPACEGATE

Spacegate is a cooperative online game written in C++ using Qt and Protobuf.

You are on a spaceship with your friends. Solve minigames to stay alive!

![Menu](https://github.com/niki4smirn/space-gate/blob/readme/main_menu.png?raw=true)
![Game](https://github.com/niki4smirn/space-gate/blob/readme/game.png?raw=true)
![Terminal](https://github.com/niki4smirn/space-gate/blob/readme/terminal.jpg?raw=true)

### Prerequisites

To run project you need at least 5.15.2 QT and 3.15.5 libprotoc.

### Installing

Clone project from repository. Build project with Cmake on MINGW or Clang, link it with QT and Protobuf.

Run server. By default, the game server will be running on localhost:1337.
Then run cleint with ip string as argument to connect to custom server or default (176.53.160.34), otherwise it will try to connect to localhost.


## Authors

* **[Nikita Smirnov](https://github.com/niki4smirn)**
    - *Server-room-clients interaction*
    - *Protobuf messages architecture (+@DudkoAndrei)*
    - *Logging system*
    - *Deployment using Docker*

* **[Andrei Dudko](https://github.com/DudkoAndrei)**
    - *CI*
    - *Server Game Controller (+@niki4smirn)*
    - *Terminal Minigame*

* **[Egor Lechenko](https://github.com/YahorL)**
    - *Main menu*
    - *Stars background animation*
    - *Hole repair minigame*
    - *Input Controller*

* **[Ekaterina Novikova](https://github.com/ekkatrina)**
    - *Game design*
    - *Animations*
    - *Menus implementation*
    - *Sound*
