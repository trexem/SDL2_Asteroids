#OBJS specifies which files to compile as part of the project
OBJS = sdl_test.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sdl_test.exe

#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) -lSDL2main -lSDL2 $(pkg-config --cflags --libs  sdl2 SDL2_mixer SDL2_image ) -o $(OBJ_NAME) 
