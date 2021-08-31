#include "game.hpp"

//gives me error for compiling with g++ for windows app, so i do this
#ifdef main
# undef main
#endif /* main */

int main(int argc, char const *argv[]) {
	Game game;
	bool is_initialized = game.initialize("Asteroids - by trexem",
	                                      SDL_WINDOWPOS_CENTERED,
	                                      SDL_WINDOWPOS_CENTERED,
	                                      SCREEN_WIDTH,
	                                      SCREEN_HEIGHT,
	                                      SDL_WINDOW_SHOWN
	                                      );
	if (!is_initialized) {
		std::cout << "Failed to initialize" << '\n';
	} else{
		bool is_media_loaded = game.loadMedia();
		if (!is_media_loaded) {
			std::cout << "Failed to load media!" << '\n';
		} else{
			game.start();
			while (!game.quit) {
				game.gameLoop();

			}//main loop

		}
	}

	return 0;
}//Main function
