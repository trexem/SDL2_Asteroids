#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "Texture.hpp"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Texture g_texture = Texture(g_renderer);

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
			SDL_Rect* pos;
			int speed = 3;
			pos->x = SCREEN_WIDTH / 2 - g_texture.getWidth() / 2;
			pos->y = SCREEN_HEIGHT / 2 - g_texture.getHeight() / 2;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.sym == SDLK_UP) {
							pos->y -= speed;
							if (pos->y < 0) {
								pos->y = 0;
							}
						} else if (e.key.keysym.sym == SDLK_DOWN) {
							pos->y += speed;
							if (pos->y > SCREEN_HEIGHT - g_texture.getHeight()) {
								pos->y = SCREEN_HEIGHT - g_texture.getHeight();
							}
						}
						if (e.key.keysym.sym == SDLK_LEFT) {
							pos->x -= speed;
							if (pos->x < 0) {
								pos->x = 0;
							}
						} else if (e.key.keysym.sym == SDLK_RIGHT) {
							pos->x += speed;
							if (pos->x > SCREEN_WIDTH - g_texture.getWidth()) {
								pos->x = SCREEN_WIDTH - g_texture.getWidth();
							}
						}
					}
				}
				SDL_RenderClear(g_renderer);
				g_texture.render(pos->x, pos->y);
				SDL_RenderPresent(g_renderer);
			}
		}
	}
	close();


	return 0;
}
