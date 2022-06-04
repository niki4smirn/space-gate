# SPACEGATE

Spacegate is cooperative online game written in C++ using Qt and Protobuf.

You are on a spaceship with your friends. Solve minigames to stay alive!

![Menu](https://github.com/niki4smirn/space-gate/blob/main/main_menu.png?raw=true)
![Game](https://github.com/niki4smirn/space-gate/blob/main/game.png?raw=true)
![Terminal](https://github.com/niki4smirn/space-gate/blob/main/terminal.jpg?raw=true)

### Prerequisites

To run project you need at least 5.15.2 QT and 3.15.5 libprotoc.

### Installing

Clone project from repository. Build project with Cmake on MINGW or Clang, link it with QT and Protobuf.

Run server. By default, the game server will be running on localhost:1337.
Then run cleint with ip string as argument to connect to custom server or default (176.53.160.34), otherwise it will try to connect to localhost.


## Authors

* **Nikita Smirnov**
    - *Server-room-clients interaction*
    - *Protobuf messages architecture (+@DudkoAndrei)*
    - *Logging system*
    - *Deployment using Docker*

  [niki4smirn](https://github.com/niki4smirn)
* **Andrei Dudko**
    - *CI*
    - *Server Game Controller (+@niki4smirn)*
    - *Terminal Minigame*

  [DudkoAndrei](https://github.com/DudkoAndrei)
* **Egor Lechenko**
    - *Main menu*
    - *Stars background animation*
    - *Hole repair minigame*
    - *Input Controller*

  [YahorL](https://github.com/YahorL)
* **Ekaterina Novikova**
    - *Game design*
    - *Animations*
    - *Menus implementation*
    - *Sound*

  [ekkatrina](https://github.com/ekkatrina)

![funny-cat-gif](https://user-images.githubusercontent.com/33293845/159215740-9512d9cf-1c9b-4663-8eb1-8a03f39ddaa8.gif)
