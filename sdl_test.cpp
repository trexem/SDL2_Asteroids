#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "Texture.hpp"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int TOP_SPEED = 1000;
const int MIN_SPEED = -50;
const int TOP_ROTATION_SPEED = 720;
const double PI = 3.14159264;


SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Texture g_texture = Texture(g_renderer);
Uint32 last_tick = 0, tick = 0;

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

bool init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	} else {
		g_window = SDL_CreateWindow("Test Interface",
		                            SDL_WINDOWPOS_CENTERED,
		                            SDL_WINDOWPOS_CENTERED,
		                            SCREEN_WIDTH,
		                            SCREEN_HEIGHT,
		                            SDL_WINDOW_SHOWN
		                            );
		if (g_window == nullptr) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		} else{
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
			if (g_renderer == nullptr) {
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			} else {
				g_texture.m_renderer = g_renderer;
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
				int img_flags = IMG_INIT_PNG;
				if (!(IMG_Init(img_flags) & img_flags)) {
					std::cout << "SDL_image could not initialize! SDL_image ERROR: " << IMG_GetError() << '\n';
				}
			}
		}
	}
	return success;
}
bool loadMedia() {
	    //Loading success flag
	bool success = true;

	    //Load splash image
	if (!g_texture.loadFromFile("img/hello_world.bmp"))
	{
		printf("Unable to load image %s! SDL Error: %s\n", "img/hello_world.bmp", SDL_GetError() );
		success = false;
	}

	return success;
}


void close() {
	g_texture.free();
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	g_renderer = nullptr;
	IMG_Quit();
	SDL_Quit();
}

void move(double* x, double* y, int* speed, double* degrees, double* speed_rotation) {
	last_tick = tick;
	tick = SDL_GetTicks();
	double time = (tick - last_tick) / 1000.0;
	double radians = *degrees * PI / 180;
	if (*speed > TOP_SPEED) {
		*speed = TOP_SPEED;
	} else if (*speed < MIN_SPEED) {
		*speed = MIN_SPEED;
	}
	if (*speed_rotation > TOP_ROTATION_SPEED) {
		*speed_rotation = TOP_ROTATION_SPEED;
	} else if (*speed_rotation < -TOP_ROTATION_SPEED) {
		*speed_rotation = -TOP_ROTATION_SPEED;
	}
	*x += *speed * time * sin(radians);
	if (*x > SCREEN_WIDTH - g_texture.getWidth()) {
		*x = SCREEN_WIDTH - g_texture.getWidth();
		*speed = 0;
		*speed_rotation = 0;
	} else if (*x < 0) {
		*x = 0;
		*speed = 0;
		*speed_rotation = 0;
	}
	*y -= *speed * time * cos(radians);
	if (*y > SCREEN_HEIGHT - g_texture.getHeight()) {
		*y = SCREEN_HEIGHT - g_texture.getHeight();
		*speed = 0;
		*speed_rotation = 0;
	} else if (*y < 0) {
		*y = 0;
		*speed = 0;
		*speed_rotation = 0;
	}
	*degrees += *speed_rotation * time;
	if (*degrees > 180) {
		*degrees -= 360;
	} else if (*degrees < -180) {
		*degrees += 360;
	}

	std::cout << "time: " << time << "\tdegrees: " << *degrees << "\tspeed rotation: " << *speed_rotation << '\n';
}


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
			bool quit = false;
			SDL_Event e;
			double speed_rotation = 0;
			double degrees = 0;
			SDL_RendererFlip flip_type = SDL_FLIP_NONE;
			double x, y;
			int speed = 0;
			x = SCREEN_WIDTH / 2 - g_texture.getWidth() / 2;
			y = SCREEN_HEIGHT / 2 - g_texture.getHeight() / 2;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}
				const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
				if (current_key_states[SDL_SCANCODE_UP]) {
					speed += 15;
					if (current_key_states[SDL_SCANCODE_LEFT]) {
						speed_rotation -= 5;
					} else if (current_key_states[SDL_SCANCODE_RIGHT]) {
						speed_rotation += 5;
					}
				} else if (current_key_states[SDL_SCANCODE_DOWN]) {
					speed -= 5;
					if (current_key_states[SDL_SCANCODE_LEFT]) {
						speed_rotation -= 5;
					} else if (current_key_states[SDL_SCANCODE_RIGHT]) {
						speed_rotation += 5;
					}
				} else if (!current_key_states[SDL_SCANCODE_UP] && !current_key_states[SDL_SCANCODE_DOWN]) {
					speed *= 0.95;
					if (speed < 1 && speed > -1) {
						speed = 0;
					}
				}
				if (!current_key_states[SDL_SCANCODE_LEFT] && !current_key_states[SDL_SCANCODE_RIGHT]) {
					speed_rotation *= 0.95;
					if (speed_rotation < 1 && speed_rotation > -1) {
						speed_rotation = 0;
					}
				}
				if (current_key_states[SDL_SCANCODE_LEFT]) {
					speed_rotation -= 5;
				} else if (current_key_states[SDL_SCANCODE_RIGHT]) {
					speed_rotation += 5;
				}
				move(&x, &y, &speed, &degrees, &speed_rotation);
				SDL_RenderClear(g_renderer);
				g_texture.renderEx((int)x, (int)y, nullptr, degrees, nullptr, flip_type);
				SDL_RenderPresent(g_renderer);
			}
		}
	}
	close();


	return 0;
}
