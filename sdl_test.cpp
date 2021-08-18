#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//Constants
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const double PI = 3.14159264;

#include "Timer.hpp"
#include "Ship.hpp"

//Global variables, prefix g_ for them
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Texture g_texture = Texture(g_renderer);
Texture g_fps_text_texture = Texture(g_renderer);
Uint32 last_tick = 0, tick = 0;
TTF_Font* g_font = nullptr;

/*SDL_Surface* loadSurface(std::string path) {
    SDL_Surface* optimized_surface = nullptr;
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == NULL) {
        std::cout << "Unable to load image " << path.c_str() << "! SDL ERROR: " << SDL_GetError() << '\n';
    } else{
        optimized_surface = SDL_ConvertSurface(loaded_surface, g_screen_surface->format, 0);
        if (optimized_surface == NULL) {
            std::cout << "Unable to optimize image " << path.c_str() << "! SDL Error: " << SDL_GetError() << '\n';
        }
        SDL_FreeSurface(loaded_surface);
    }
    return optimized_surface;
   }*/
SDL_Texture* loadTexture(std::string path) {
	SDL_Texture* new_texture = nullptr;
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == nullptr) {
		std::cout << "Unable to load image " << path.c_str() << "! SDL ERROR: " << SDL_GetError() << '\n';
	} else{
		new_texture = SDL_CreateTextureFromSurface(g_renderer, loaded_surface);
		if (new_texture == nullptr) {
			std::cout << "Unable to create texture from " << path.c_str() << "! SDL Eerror: " << SDL_GetError() << '\n';
		}
		SDL_FreeSurface(loaded_surface);
	}
	return new_texture;
}
//Initialize SDLvideo and SDLimage
bool init() {
	    //Flag to know if everything is ok
	bool success = true;
	    //Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		    //If error, print error
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	} else {
		    //Create SDL window
		g_window = SDL_CreateWindow("Test Interface",
		                            SDL_WINDOWPOS_CENTERED,
		                            SDL_WINDOWPOS_CENTERED,
		                            SCREEN_WIDTH,
		                            SCREEN_HEIGHT,
		                            SDL_WINDOW_SHOWN
		                            );
		if (g_window == nullptr) {
			    //if error while creating window, print error
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		} else{
			    //create renderer for g_window
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
			if (g_renderer == nullptr) {
				    //if error whielcreating renderer, print error
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			} else {
				    //as of right now i pass herethe renderer to the texture, thinking of doing it differently
				g_texture.m_renderer = g_renderer;
				g_fps_text_texture.m_renderer = g_renderer;
				    //start screen in black
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
				int img_flags = IMG_INIT_PNG;
				    //init SDL_image
				if (!(IMG_Init(img_flags) & img_flags)) {
					    //print error
					std::cout << "SDL_image could not initialize! SDL_image ERROR: " << IMG_GetError() << '\n';
					success = false;
				}
				    //init SDL_ttf
				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
					success = false;
				}
			}
		}
	}
	return success;
}
//Function to load all Media to be used
bool loadMedia() {//Thinking that loadMedia should be a function for each class/object that needs to load
	//something,so we don't use a global variable for the texture and instead have the texture object
	//inside the other object, in this case the text object or the spaceship object

	    //Loading success flag
	bool success = true;

	g_font = TTF_OpenFont("fonts/consola.ttf", 24);
	if (g_font == NULL) {
		std::cout << "Failed to load consola font! SDL_ttf Error:" << TTF_GetError() << '\n';
		success = false;
	}

	return success;
}

//Free memory and close properly
void close() {
	g_texture.free();
	g_fps_text_texture.free();
	TTF_CloseFont(g_font);
	g_font = nullptr;
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	g_renderer = nullptr;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

//Motion function


//gives me error for compiling with g++ for windows app, so i do this
#ifdef main
# undef main
#endif /* main */

int main(int argc, char const *argv[]) {
	if (!init()) {
		std::cout << "Failed to initialize" << '\n';
	} else{
		if (!loadMedia()) {
			std::cout << "Failed to load media!" << '\n';
		} else{
			    //initialize variables to use
			bool quit = false;
			SDL_Event e;
			Ship spaceship = Ship(g_renderer);
			spaceship.setPos(SCREEN_WIDTH / 2 - g_texture.getWidth() / 2,
			                 SCREEN_HEIGHT / 2 - g_texture.getHeight() / 2,
			                 0);
			SDL_Color text_color = {0, 255, 255, 255};
			Timer fps_timer, cap_timer, step_timer;
			std::stringstream time_text;
			int counted_frames = 0;
			fps_timer.start();
			double time_step;

			    //start main loop
			while (!quit) {
				cap_timer.start();
				while (SDL_PollEvent(&e) != 0) {
					    //Quit if the X button is pressed
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}
				    //The array current_key_states has the state of the pressed keys
				const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
				    //Key UP
				spaceship.handleInput(current_key_states);
				//calculate fps

				float avg_fps = counted_frames / (fps_timer.getTicks() / 1000.f);
				if (avg_fps > 9999) {
					avg_fps = 0;
				}
				time_text.str("");
				time_text << "Average FPS: " << avg_fps;
				if (!g_fps_text_texture.loadFromRenderedText(time_text.str().c_str(), text_color, g_font)) {
					std::cout << "Unable to render FPS texture!" << '\n';
				}
				//Call to motion function

				time_step = step_timer.getTicks() / 1000.0;
				spaceship.move(time_step);
				step_timer.start();

				    //Clear renderer
				SDL_RenderClear(g_renderer);
				    //Render the spaceship
				spaceship.render();
				g_fps_text_texture.render(0, 0);
				//g_texture.renderEx((int)spaceship.getX(), (int)spaceship.getY(), nullptr, spaceship.getDegrees(), nullptr, flip_type);

				    //display in window the render
				SDL_RenderPresent(g_renderer);
				++counted_frames;
				int frame_ticks = cap_timer.getTicks();
				if (frame_ticks < SCREEN_TICKS_PER_FRAME)
				{
					    //Wait remaining time
					SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
				}
			}//main loop
		}
	}
	close();


	return 0;
}//Main function
