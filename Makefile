#OBJS specifies which files to compile as part of the project
OBJS = src/AsteroidsGame.o src/ship.o src/texture.o src/timer.o src/window.o src/renderer.o src/game.o

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ./release/AsteroidsGame


#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#This is the target that compiles our executable
all : $(OBJS)
	cp -r ./data ./release/
	g++ $(OBJS)  $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -o $(OBJ_NAME) -O3

src/timer.o: src/timer.cpp src/timer.hpp
	g++ src/timer.cpp  $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c -o ./$@

src/texture.o: src/texture.cpp src/texture.hpp
	g++ src/texture.cpp  $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c -o ./$@

src/window.o: src/window.cpp src/window.hpp
	g++ src/window.cpp  $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c -o ./$@

src/renderer.o: src/renderer.cpp src/renderer.hpp
	g++ src/renderer.cpp  $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c -o ./$@

src/ship.o: src/ship.cpp src/ship.hpp src/texture.hpp
	g++ src/ship.cpp  $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c -o ./$@

src/game.o: src/game.cpp src/game.hpp
	g++ src/game.cpp  $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c -o ./$@

src/AsteroidsGame.o: src/AsteroidsGame.cpp
	g++ src/AsteroidsGame.cpp $(LINKER_FLAGS) $(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) \
	-g -c -o ./$@

clean:
	rm -rf ./release/AsteroidsGame
	rm -rf ./src/*.o
