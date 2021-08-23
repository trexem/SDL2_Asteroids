#ifndef _TIMER_H_
#define _TIMER_H_
#include <SDL2/SDL.h>

class Timer {
public:
	Timer() {
		m_start_ticks = 0;
		m_paused_ticks = 0;
		m_paused = false;
		m_started = false;
	}

	void start() {
		m_started = true;
		m_paused = false;
		m_start_ticks = SDL_GetTicks();
		m_paused_ticks = 0;
	}
	void stop() {
		m_started = false;
		m_paused = false;
		m_start_ticks = 0;
		m_paused_ticks = 0;
	}
	void pause() {
		if (m_started && !m_paused) {
			m_paused = true;
			m_paused_ticks = SDL_GetTicks() - m_start_ticks;
			m_start_ticks = 0;
		}
	}
	void unpause() {
		if (m_started && m_paused) {
			m_paused = false;
			m_start_ticks = SDL_GetTicks() - m_paused_ticks;
			m_paused_ticks = 0;
		}
	}

	Uint32 getTicks() {
		if (m_started) {
			if (m_paused) {
				return 0;
			} else {
				return SDL_GetTicks() - m_start_ticks;
			}
		}
		return 0;
	}

	bool isStarted() {
		return m_started;
	}
	bool isPaused() {
		return m_paused;
	}

private:
	Uint32 m_start_ticks {
		0
	};
	Uint32 m_paused_ticks{0};
	bool m_paused{false};
	bool m_started{false};
};

#endif
