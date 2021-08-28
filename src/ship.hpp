#ifndef _SHIP_H_
#define _SHIP_H_

#include "texture.hpp"
const int SHIP_MAX_SHOTS = 10;
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const double PI = 3.14159264;

class Shot {
public:
	    //Speed for the shot, must be faster than the max speed of spaceship
	const int SHOT_SPEED = 1000;
//Constructor, we have to give initial position, rotation and give a texture
	Shot(int t_x, int t_y, double t_degrees, Texture* t_texture);

//Move function, needs the time that has passed since last movement
	void move(double t_time_step);

	    //Set Position x, y and rotation
	void setPos(double t_x, double t_y, double t_rot_degrees);

	    //Renders if it is not dead
	void render(void);

	    //Returns true if shot is outside of the window + 15 pixels to be able to see the tail
	bool isDead(void);

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
	Ship(SDL_Renderer * t_renderer);
	    //Desctructor, we also delete all shots
	~Ship(void);
	    //Handle inputs that control the spaceship
	void handleInput(const Uint8* t_current_key_states);
	    //Move function, needs the time that has passed since last movement
	void move(double t_time_step);
//Set position function, x, y and rotation
	void setPos(double t_x, double t_y, double t_rot_degrees);
	    //restart spaceship to the center, also restarts shots
	void restart(void);
	    //Render function in
	void render(void);
	    //Shoot a singleshot from the tip of the spaceship
	void shoot(void);
	    //free textures
	void free(void);
	    //get x position (top left corner)
	double getX();
	    //get y position (top left corner)
	double getY();
	    //get degrees of rotation
	double getDegrees();
	    //get the texture of the ship
	Texture getTexture();
private:
	    //Calls to render all shots that are not dead
	void renderShots();
	Shot* m_shots[SHIP_MAX_SHOTS];//pointer array of shots
	double m_pos_x{0}, m_pos_y{0}, m_rot_degrees{0};
	double m_vel{0}, m_rot_vel{0};
	Texture m_texture, m_shot_texture;
	SDL_Renderer* m_renderer;
	double m_last_shot;
};
#endif
