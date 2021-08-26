# ASTEROIDS GAME by trexem
Mostly testing and playing with SDL2, it is still WIP so be patient, I'm learning about it.

You need to have installed SDL2 libraries, I'm using `g++` compiler in `cygwin64` and `make` command to compile. But there're tutorials for you to install SDL2 with MingW32, Codeblocks or whatever, also check [SDL official page](https://www.libsdl.org/)

* [AsteroidsGame.cpp](https://github.com/trexem/SDL2_Asteroids/blob/main/AsteroidsGame.cpp) is the main file
* [Ship.hpp](https://github.com/trexem/SDL2_Asteroids/blob/main/Ship.hpp) has the Shot and Ship classes
* [Texture.hpp](https://github.com/trexem/SDL2_Asteroids/blob/main/Texture.hpp) has the Texture class, which is used to load images and text into the screen
* [Timer.hpp](https://github.com/trexem/SDL2_Asteroids/blob/main/Timer.hpp) has a simple timer class, you can pause, resume and restart timer
* [Makefile](https://github.com/trexem/SDL2_Asteroids/blob/main/Makefile) is used to compile using `make` command, check it out to see which SDL libraries are being used.

## Try out
Download the `/release` folder and run `AsteroidsGame.exe`
