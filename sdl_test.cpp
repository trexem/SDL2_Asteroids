#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

SDL_Window* g_window = nullptr;
SDL_Surface* g_screen_surface = nullptr;
SDL_Surface* g_hello_world = nullptr;


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
			g_screen_surface = SDL_GetWindowSurface(g_window);
		}
	}
	return success;
}
bool loadMedia()
{
	    //Loading success flag
	bool success = true;

	    //Load splash image
	g_hello_world = SDL_LoadBMP("img/hello_world.bmp");
	if (g_hello_world == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "img/hello_world.bmp", SDL_GetError() );
		success = false;
	}

	return success;
}

void close() {
	SDL_FreeSurface(g_hello_world);
	g_hello_world = nullptr;
	SDL_DestroyWindow(g_window);
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
			pos->x = g_screen_surface->clip_rect.w / 2 - g_hello_world->clip_rect.w / 2;
			pos->y = g_screen_surface->clip_rect.h / 2 - g_hello_world->clip_rect.h / 2;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
						case SDLK_UP:
							pos->y -= speed;
							if (pos->y < 0) {
								pos->y = 0;
							}
							break;
						case SDLK_DOWN:
							pos->y += speed;
							if (pos->y > SCREEN_HEIGHT - g_hello_world->clip_rect.h) {
								pos->y = SCREEN_HEIGHT - g_hello_world->clip_rect.h;
							}
							break;
						case SDLK_LEFT:
							pos->x -= speed;
							if (pos->x < 0) {
								pos->x = 0;
							}
							break;
						case SDLK_RIGHT:
							pos->x += speed;
							if (pos->x > SCREEN_WIDTH - g_hello_world->clip_rect.w) {
								pos->x = SCREEN_WIDTH - g_hello_world->clip_rect.w;
							}
							break;
						}
					}
				}
				SDL_FillRect(g_screen_surface, nullptr, SDL_MapRGB(g_screen_surface->format, 0xFF, 0xFF, 0xFF));
				SDL_BlitSurface(g_hello_world, nullptr, g_screen_surface, pos);
				SDL_UpdateWindowSurface(g_window);
			}
		}
	}
	close();


	return 0;
}
