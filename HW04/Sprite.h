#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

using namespace std;

class Sprite {
public:
	Sprite(GLuint id, float x, float y, float u, float v, float w, float h, float vX = 0.0f, float vY = 0.0f, float aX = 0.0f, float aY = 0.0f, float fX = 1.2f, float fY = 1.2f, bool isStatic = true);
	void Render();
	bool collidesWith(Sprite *entity);
	GLuint id;
	float x;
	float y;
	float u;
	float v;
	float width;
	float height;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;	
	float friction_y;
	bool isVisible;
	bool isStatic;
	bool enableCollisions;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
};

