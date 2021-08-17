#OBJS specifies which files to compile as part of the project
OBJS = sdl_test.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sdl_test.exe

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\cygwin64\usr\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\cygwin64\lib

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -g -o $(OBJ_NAME)
