A Qt5/C++ based Minesweeper clone
=================================

Note on compiling from source: in Linux, the Qt version 5.15.2 may produce a buggy game executable - clicking on a button and moving the mouse moves the whole window.
Qt versions 5.12.12, 5.14.2 and 6.2.2 work fine. The Windows and macOS versions are not affected.

Latest release:
---------------
|Filename|Platform|
|:---|:---|
|[qminesweeper.exe](https://github.com/q-g-j/qminesweeper/releases/download/latest/qminesweeper.exe)|Windows - 32 bit static build|
|[qminesweeper.dmg](https://github.com/q-g-j/qminesweeper/releases/download/latest/qminesweeper.dmg)|macOS - 64 bit|
|[qminesweeper.AppImage](https://github.com/q-g-j/qminesweeper/releases/download/latest/qminesweeper.AppImage)|Linux - 64 bit AppImage|

Controls:
---------

|Mouse button|Action|
|-|-|
|Left click|reveal a square|
|Right click|place or remove a flag|
|Left + right click on a number|automatically reveal linked squares with no mine (if flags are placed correctly) |
|Left click on smiley|start a new game with the same difficulty|

|Key|Action|
|-|-|
|'F'|let the computer place the flags|
|'R'|auto-reveal after placing flags|
|'S'|the computer tries to solve the game (repeats the above two recursively)|

Screenshots:
-----------

<img src="https://github.com/q-g-j/qminesweeper/raw/master/screenshots/screenshot_game.png" width="500">

<img src="https://github.com/q-g-j/qminesweeper/raw/master/screenshots/screenshot_lost.png" width="500">

<img src="https://github.com/q-g-j/qminesweeper/raw/master/screenshots/screenshot_won.png" width="500">
