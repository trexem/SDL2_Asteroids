#ifndef _SHIP_H_
#define _SHIP_H_

#include "Texture.hpp"
const int SHIP_MAX_SHOTS = 10;

class Shot {
public:
	    //Speed for the shot, must be faster than the max speed of spaceship
	const int SHOT_SPEED = 1000;
//Constructor, we have to give initial position, rotation and give a texture
	Shot(int t_x, int t_y, double t_degrees, Texture* t_texture) {
		m_pos_x = t_x;
		m_pos_y = t_y;
		m_rot_degrees = t_degrees;
		m_texture = t_texture;
	}
//Move function, needs the time that has passed since last movement
	void move(double t_time_step) {
		if (!isDead()) {
			    //Convert degrees to radians
			double radians = m_rot_degrees * PI / 180;
			m_pos_x += SHOT_SPEED * t_time_step * sin(radians);
			m_pos_y -= SHOT_SPEED * t_time_step * cos(radians);
		}
	}
	    //Set Position x, y and rotation
	void setPos(double t_x, double t_y, double t_rot_degrees) {
		m_pos_x = t_x;
		m_pos_y = t_y;
		m_rot_degrees = t_rot_degrees;
	}
	    //Renders if it is not dead
	void render() {
		if (!isDead()) {
			m_texture->renderEx(m_pos_x, m_pos_y, nullptr, m_rot_degrees, nullptr, SDL_FLIP_NONE);
		}
	}
	    //Returns true if shot is outside of the window + 15 pixels to be able to see the tail
	bool isDead() {
		if (m_pos_x > SCREEN_WIDTH + 15 || m_pos_x < -15 || m_pos_y > SCREEN_HEIGHT + 15 || m_pos_y < -15) {
			return true;
		}
		return false;
	}
private:
	int m_pos_x{0}, m_pos_y{0};
	double m_rot_degrees{0};
	Texture* m_texture;
};

class Ship {
public:
	    //TOP speed of ship, must be lower than the speed of shot
	const int SHIP_TOP_SPEED = 500;
	    //Min speed in reverse (lower than forward speed)
	const int SHIP_MIN_SPEED = -200;
	const int SHIP_TOP_ROTATION_SPEED = 540; //1.5 turn per second
	const int SHIP_SPEED = 15; //acceleration, instead of speed
	const int SHIP_ROT_SPEED = 10; //acceleration, instead of speed
	const float SHIP_SHOT_DELAY = .25; //How fast will the ship shoot?
	    //Constructor, only needs a pointer to  set the renderer
	Ship(SDL_Renderer * t_renderer) {
		m_pos_x = 0;
		m_pos_y = 0;
		m_rot_degrees = 0;
		m_vel = 0;
		m_rot_vel = 0;
		m_renderer = t_renderer;
		m_texture = Texture(m_renderer);
		m_shot_texture = Texture(m_renderer);
		m_texture.loadFromFile("img/spaceship.bmp"); //initialize and pass image for the ship
		m_shot_texture.loadFromFile("img/shot.png");//initialize and pass image for the shot
		    //initialize shots outside of the scree, so "dead" shots
		for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
			m_shots[i] = new Shot(-20, -20, 0, &m_shot_texture);
		}
	}
	    //Desctructor, we also delete all shots
	~Ship() {
		for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
			delete m_shots[i];
		}
	}
	    //Handle inputs that control the spaceship
	void handleInput(const Uint8* t_current_key_states) {
		    //There are so many ifs because it moved weirdly with separate ifs for each key
		    //So i have to add the cases for more than one key pressed at the same time
		if (t_current_key_states[SDL_SCANCODE_UP] || t_current_key_states[SDL_SCANCODE_W]) {
			m_vel += SHIP_SPEED;
			    //Key LEFT
			if (t_current_key_states[SDL_SCANCODE_LEFT] || t_current_key_states[SDL_SCANCODE_A]) {
				if (m_rot_vel > 0) {
					m_rot_vel = 0;
				}
				m_rot_vel -= SHIP_ROT_SPEED;
			} else if (t_current_key_states[SDL_SCANCODE_RIGHT] || t_current_key_states[SDL_SCANCODE_D]) { //Key RIGHT
				if (m_rot_vel < 0) {
					m_rot_vel = 0;
				}
				m_rot_vel += SHIP_ROT_SPEED;
			}
		} else if (t_current_key_states[SDL_SCANCODE_DOWN] || t_current_key_states[SDL_SCANCODE_S]) { //Key DOWN
			m_vel -= SHIP_SPEED;
			if (t_current_key_states[SDL_SCANCODE_LEFT] || t_current_key_states[SDL_SCANCODE_A]) {
				if (m_rot_vel > 0) {
					m_rot_vel = 0;
				}
				m_rot_vel -= SHIP_ROT_SPEED;
			} else if (t_current_key_states[SDL_SCANCODE_RIGHT] || t_current_key_states[SDL_SCANCODE_D]) {
				if (m_rot_vel < 0) {
					m_rot_vel = 0;
				}
				m_rot_vel += SHIP_ROT_SPEED;
			}
		} else if (!t_current_key_states[SDL_SCANCODE_UP] && !t_current_key_states[SDL_SCANCODE_DOWN] &&
		           !t_current_key_states[SDL_SCANCODE_W] && !t_current_key_states[SDL_SCANCODE_S]) {
			    //If no UP or DOWN are pressed we desacelerate
			m_vel *= 0.95;
			if (m_vel < 1 && m_vel > -1) { //if the number is too small we round down to 0
				m_vel = 0;
			}
		}
		if (!t_current_key_states[SDL_SCANCODE_LEFT] && !t_current_key_states[SDL_SCANCODE_RIGHT] &&
		    !t_current_key_states[SDL_SCANCODE_A] && !t_current_key_states[SDL_SCANCODE_D]) {
			    //If no LEFT or RIGHT are pressed we desacelerate the turn
			m_rot_vel *= 0.95;
			if (m_rot_vel < 1 && m_rot_vel > -1) { //if the number is too small we round down to 0
				m_rot_vel = 0;
			}
		}
		if (t_current_key_states[SDL_SCANCODE_LEFT] || t_current_key_states[SDL_SCANCODE_A]) {
			if (m_rot_vel > 0) {
				m_rot_vel = 0;
			}
			m_rot_vel -= SHIP_ROT_SPEED;
		} else if (t_current_key_states[SDL_SCANCODE_RIGHT] || t_current_key_states[SDL_SCANCODE_D]) {
			if (m_rot_vel < 0) {
				m_rot_vel = 0;
			}
			m_rot_vel += SHIP_ROT_SPEED;
		}
		if (t_current_key_states[SDL_SCANCODE_SPACE] && m_last_shot > SHIP_SHOT_DELAY) {
			shoot();
			m_last_shot = 0;
		}
	}
	    //Move function, needs the time that has passed since last movement
	void move(double t_time_step) {
		m_last_shot += t_time_step;
		for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
			m_shots[i]->move(t_time_step);
		}
		    //convert degrees to radians
		double radians = m_rot_degrees * PI / 180;
		    //top max and min speed
		if (m_vel > SHIP_TOP_SPEED) {
			m_vel = SHIP_TOP_SPEED;
		} else if (m_vel < SHIP_MIN_SPEED) {
			m_vel = SHIP_MIN_SPEED;
		}
		    //top max and min rotation speed
		if (m_rot_vel > SHIP_TOP_ROTATION_SPEED) {
			m_rot_vel = SHIP_TOP_ROTATION_SPEED;
		} else if (m_rot_vel < -SHIP_TOP_ROTATION_SPEED) {
			m_rot_vel = -SHIP_TOP_ROTATION_SPEED;
		}
		    //move X
		m_pos_x += m_vel * t_time_step * sin(radians);
		if (m_pos_x > SCREEN_WIDTH - m_texture.getWidth()) {
			m_pos_x = SCREEN_WIDTH - m_texture.getWidth();
			m_vel = 0;
			m_rot_vel = 0;
		} else if (m_pos_x < 0) {
			m_pos_x = 0;
			m_vel = 0;
			m_rot_vel = 0;
		}
		    //move Y, -= because Y axis is inverted
		m_pos_y -= m_vel * t_time_step * cos(radians);
		if (m_pos_y > SCREEN_HEIGHT - m_texture.getHeight()) {
			m_pos_y = SCREEN_HEIGHT - m_texture.getHeight();
			m_vel = 0;
			m_rot_vel = 0;
		} else if (m_pos_y < 0) {
			m_pos_y = 0;
			m_vel = 0;
			m_rot_vel = 0;
		}
		    //move degrees
		m_rot_degrees += m_rot_vel * t_time_step;
		    //to avoid going to infinite degrees we stay between -180 and 180
		if (m_rot_degrees > 180) {
			m_rot_degrees -= 360;
		} else if (m_rot_degrees < -180) {
			m_rot_degrees += 360;
		}

	}
//Set position function, x, y and rotation
	void setPos(double t_x, double t_y, double t_rot_degrees) {
		m_pos_x = t_x;
		m_pos_y = t_y;
		m_rot_degrees = t_rot_degrees;
	}
	    //restart spaceship to the center, also restarts shots
	void restart() {
		m_pos_x = SCREEN_WIDTH / 2;
		m_pos_y = SCREEN_HEIGHT / 2;
		m_rot_degrees = 0;
		m_vel = 0;
		m_rot_vel = 0;
		for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
			m_shots[i]->setPos(-20, -20, 0);
		}
	}
	    //Render function in
	void render() {
		    //x, y, null to show the entire ship, degrees, null to take the center as the pivot to rotate, and no flipped image
		m_texture.renderEx(m_pos_x, m_pos_y, nullptr, m_rot_degrees, nullptr, SDL_FLIP_NONE);
		renderShots();
	}
	    //Shoot a singleshot from the tip of the spaceship
	void shoot() {
		for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
			if (m_shots[i]->isDead()) {
				    //we delete current shot if is dead
				delete m_shots[i];
				double radians = m_rot_degrees * PI / 180;
				int x = (int) (m_pos_x + m_texture.getWidth() / 2 + sin(radians) * m_texture.getHeight() / 2);
				int y = (int) (m_pos_y + m_texture.getHeight() / 2 - cos(radians) * m_texture.getHeight() / 2);
				    //create new shot with the position of the tip off the spaceship
				m_shots[i] = new Shot(x, y, m_rot_degrees, &m_shot_texture);
				i = SHIP_MAX_SHOTS; //could also be an "exit for" or "break"
			}
		}
	}
	    //free textures
	void free() {
		m_texture.free();
		m_shot_texture.free();
	}
	    //get x position (top left corner)
	double getX() {
		return m_pos_x;
	}
	    //get y position (top left corner)
	double getY() {
		return m_pos_y;
	}
	    //get degrees of rotation
	double getDegrees() {
		return m_rot_degrees;
	}
	    //get the texture of the ship
	Texture getTexture() {
		return m_texture;
	}
private:
	    //Calls to render all shots that are not dead
	void renderShots() {
		for (int i = 0; i < SHIP_MAX_SHOTS; i++) {
			m_shots[i]->render();
		}
	}
	Shot* m_shots[SHIP_MAX_SHOTS];//pointer array of shots
	double m_pos_x{0}, m_pos_y{0}, m_rot_degrees{0};
	double m_vel{0}, m_rot_vel{0};
	Texture m_texture, m_shot_texture;
	SDL_Renderer* m_renderer;
	double m_last_shot;
};
#endif
