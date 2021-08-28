#include "texture.hpp"

Texture::Texture() {
	m_renderer = nullptr;
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
}

Texture::Texture(SDL_Renderer* g_renderer) {
	m_renderer = g_renderer;
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
}

Texture::~Texture() {
	free();
}

bool Texture::loadFromFile(std::string t_path) {
	free();
	SDL_Texture* new_texture = nullptr;
	auto loaded_surface = std::unique_ptr<SDL_Surface, SDL_Surface_Deleter>(IMG_Load(t_path.c_str()));
	if (!loaded_surface) {
		std::cout << "Unable to load image " << t_path.c_str() << "! SDL ERROR: " << SDL_GetError() << '\n';
	} else {
		SDL_SetColorKey(&*loaded_surface, SDL_TRUE, SDL_MapRGB(&*loaded_surface->format, 0, 0xFF, 0xFF));
		new_texture = SDL_CreateTextureFromSurface(m_renderer, &*loaded_surface);
		if (new_texture == NULL) {
			std::cout << "Unable to create texture from " << t_path.c_str() << "! SDL Eerror: " << SDL_GetError() << '\n';
		} else{
			m_width = loaded_surface->w;
			m_height = loaded_surface->h;
		}
	}
	m_texture = new_texture;
	return m_texture != nullptr;
}

bool Texture::loadFromRenderedText(std::string t_texture_text, SDL_Color t_text_color, TTF_Font* g_font) {
	free();
	auto text_surface = std::unique_ptr<SDL_Surface, SDL_Surface_Deleter>(TTF_RenderText_Solid(g_font, t_texture_text.c_str(), t_text_color));
	if (text_surface) {
		m_texture = SDL_CreateTextureFromSurface(m_renderer, &*text_surface);
		if (m_texture == NULL) {
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << '\n';
		} else {
			m_width = text_surface->w;
			m_height = text_surface->h;
		}
	} else {
		std::cout << "Unable to render text surface! SDL_ttf Error:" << SDL_GetError() << '\n';
	}
	return m_texture != NULL;
}

void Texture::free() {
	if (m_texture != nullptr) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}

void Texture::render(int t_x, int t_y) {
	SDL_Rect render_quad = {t_x, t_y, m_width, m_height};
	SDL_RenderCopy(m_renderer, m_texture, NULL, &render_quad);
}

void Texture:: renderEx(int t_x, int t_y, SDL_Rect* t_clip, double t_angle, SDL_Point* t_center, SDL_RendererFlip t_flip) {
	SDL_Rect render_quad = {t_x, t_y, m_width, m_height};
	SDL_RenderCopyEx(m_renderer, m_texture, t_clip, &render_quad, t_angle, t_center, t_flip);
}

int Texture::getWidth() {
	return m_width;
}

int Texture::getHeight() {
	return m_height;
}
