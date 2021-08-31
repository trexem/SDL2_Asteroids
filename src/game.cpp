#include "game.hpp"

Game::Game() {
	quit = false;
	pause = false;
	last_tick = 0;
	tick = 0;
}

Game::~Game() {
	m_ship->free();
	m_fps_text_texture.free();
	m_pause_text_texture.free();
	TTF_CloseFont(m_fps_ttf);
	TTF_CloseFont(m_pause_ttf);
	m_fps_ttf = nullptr;
	m_pause_ttf = nullptr;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Game::initialize(const char* t_title, int t_x, int t_y, int t_width, int t_height, Uint32 flags) {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL could not initialize! SDL_ERROR: " << SDL_GetError() << '\n';
		success = false;
	} else {
		m_window = Window(t_title, t_x, t_y, t_width, t_height, flags);
		if (!m_window.getWindow()) {
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
			success = false;
		} else {
			m_renderer = new Renderer(m_window.getWindow(), -1, SDL_RENDERER_ACCELERATED);
			if (!m_renderer->getRenderer()) {
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			} else {
				m_fps_text_texture.m_renderer = m_renderer->getRenderer();
				m_pause_text_texture.m_renderer = m_renderer->getRenderer();
				m_renderer->changeColor(0x00, 0x00, 0x00, 0xFF);
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

bool Game::loadMedia() {
	bool success = true;

	m_fps_ttf = TTF_OpenFont("data/fonts/consola.ttf", 14);
	if (m_fps_ttf == NULL) {
		std::cout << "Failed to load consola font! SDL_ttf Error:" << TTF_GetError() << '\n';
		success = false;
	}
	m_pause_ttf = TTF_OpenFont("data/fonts/consola.ttf", 58);
	if (m_pause_ttf == NULL) {
		std::cout << "Failed to load consola font! SDL_ttf Error:" << TTF_GetError() << '\n';
		success = false;
	}

	return success;
}

void Game::start() {
	m_ship = new Ship(m_renderer->getRenderer());
	m_ship->setPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	pause_text.str("");
	pause_text << "PAUSE";
	counted_frames = 0;
	if (!m_pause_text_texture.loadFromRenderedText(pause_text.str().c_str(), white_color, m_pause_ttf)) {
		std::cout << "Unable to render FPS texture!" << '\n';
	}
	    //start fps timer
	fps_timer.start();
}

void Game::gameLoop() {
	cap_timer.start(); //start cap timer at the beggining of the "frame"
	while (SDL_PollEvent(&e) != 0) {
		    //Quit if the X button is pressed
		if (e.type == SDL_QUIT) {
			quit = true;
		} else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_p) {//P for pause
				pause = !pause;
			}
			if (e.key.keysym.sym == SDLK_r) {//R for restart. Will change in the future for an option in the pause menu
				m_ship->restart();
			}
		}
	}
	    //The array current_key_states has the state of the pressed keys
	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);

	    //calculate fps: how many frames divided by the time that has passed since the game started
	float avg_fps = counted_frames / (fps_timer.getTicks() / 1000.f);
	if (avg_fps > 9999) {
		avg_fps = 0;
	}
	    //Set text for the fps
	time_text.str("");
	time_text << "Average FPS: " << avg_fps;
	if (!m_fps_text_texture.loadFromRenderedText(time_text.str().c_str(), white_color, m_fps_ttf)) {
		std::cout << "Unable to render FPS texture!" << '\n';
	}
	    //Calculate time between previous movement and now
	time_step = step_timer.getTicks() / 1000.0;
	    //while in pause, we don't take account of keys to move the spaceship
	if (!pause) {
		m_ship->handleInput(current_key_states);
	}
	    //move spaceship
	m_ship->move(time_step);
	    //restart step timer
	step_timer.start();

	    //Clear renderer
	m_renderer->clear();
	    //Render
	m_ship->render();
	m_fps_text_texture.render(0, 0);
	    //Render PAUSE text while game is paused
	if (pause) {
		step_timer.pause();
		m_pause_text_texture.render(
			SCREEN_WIDTH / 2 - m_pause_text_texture.getWidth() / 2,
			SCREEN_HEIGHT / 2 - m_pause_text_texture.getHeight() / 2
			);
	}
	    //display in window the render
	m_renderer->render();
	++counted_frames;
	    //Wait time to cap FPS at 60
	int frame_ticks = cap_timer.getTicks();
	if (frame_ticks < SCREEN_TICKS_PER_FRAME)
	{
		    //Wait remaining time
		SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
	}
}
