#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image.h>

class Texture {
public:
	Texture(SDL_Renderer* g_renderer) {
		m_renderer = g_renderer;
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
	~Texture() {
		free();
	}

	bool loadFromFile(std::string t_path) {
		free();
		SDL_Texture* new_texture = nullptr;
		SDL_Surface* loaded_surface = IMG_Load(t_path.c_str());
		if (loaded_surface == nullptr) {
			std::cout << "Unable to load image " << t_path.c_str() << "! SDL ERROR: " << SDL_GetError() << '\n';
		} else {
			SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
			new_texture = SDL_CreateTextureFromSurface(m_renderer, loaded_surface);
			if (new_texture == NULL) {
				std::cout << "Unable to create texture from " << t_path.c_str() << "! SDL Eerror: " << SDL_GetError() << '\n';
			} else{
				m_width = loaded_surface->w;
				m_height = loaded_surface->h;
			}
			SDL_FreeSurface(loaded_surface);
		}
		m_texture = new_texture;
		return m_texture != nullptr;
	}
	void free() {
		if (m_texture != nullptr) {
			SDL_DestroyTexture(m_texture);
			m_texture = nullptr;
			m_width = 0;
			m_height = 0;
		}
	}
	void render(int t_x, int t_y) {
		SDL_Rect render_quad = {t_x, t_y, m_width, m_height};
		SDL_RenderCopy(m_renderer, m_texture, NULL, &render_quad);
	}
	void renderEx(int t_x, int t_y, SDL_Rect* t_clip, double t_angle, SDL_Point* t_center, SDL_RendererFlip t_flip) {
		SDL_Rect render_quad = {t_x, t_y, m_width, m_height};
		SDL_RenderCopyEx(m_renderer, m_texture, t_clip, &render_quad, t_angle, t_center, t_flip);
	}

	int getWidth() {
		return m_width;
	}
	int getHeight() {
		return m_height;
	}

	SDL_Renderer* m_renderer{nullptr};
private:
	SDL_Texture* m_texture{nullptr};
	int m_width{0};
	int m_height{0};
};




#endif
