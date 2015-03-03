#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <math.h>
#include <vector> 

using namespace std;

class Sprite{
	float x, y, u, v, w, h, speed, direction;
	bool notDead, visible;
	GLuint texture;
public:
	Sprite(GLuint texture, float x, float y, float u, float v, float w, float h, bool visible);
	void setx(float xPos); 
	void sety(float yPos);
	void setu(float uPos);
	void setv(float vPos);
	float getDirection();
	float getx();
	float gety();
	float getSpeed();
	void drawSprite(float offset);
	bool getVisible();
	void setVisible(bool type);
	float getWidth();
	float getHeight();
	bool getStatus();
	void setStatus(bool status);
};