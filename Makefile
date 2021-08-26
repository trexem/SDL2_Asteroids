#OBJS specifies which files to compile as part of the project
OBJS = .\\src\\AsteroidsGame.o .\\src\\ship.o .\\src\\texture.o .\\src\\timer.o

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = release\AsteroidsGame.exe

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\cygwin64\usr\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\cygwin64\lib

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#This is the target that compiles our executable
all : $(OBJS)
	cp -r .\\data .\\release\\
	g++ $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -o $(OBJ_NAME) -O3

timer.o: .\\src\\timer.cpp .\\src\\timer.hpp
	g++ .\\src\\timer.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c

texture.o: .\\src\\texture.cpp .\\src\\texture.hpp
	g++ .\\src\\texture.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c

ship.o: .\\src\\ship.cpp .\\src\\ship.hpp .\\src\\texture.hpp
	g++ .\\src\\ship.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) \
	$(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -c

AsteroidsGame.o: .\\src\\AsteroidsGame.cpp .\\src\\timer.hpp .\\src\\texture.hpp .\\src\\ship.hpp
	g++ .\\src\\AsteroidsGame.cpp $(INCLUDE_PATHS) \
	$(LIBRARY_PATHS) $(LINKER_FLAGS) $(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) \
	-g -c
