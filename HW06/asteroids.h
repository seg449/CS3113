#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector> 
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "sprite.h"

enum state {MAIN_MENU, GAME, OVER};

class Asteroids{
	SDL_Window* display;
	sprite* player;
	vector<sprite*> asteroids;
	GLuint font, sheet;
	float timeLeft;
	float lastFrameTicks;
	bool finished;
	int state;
public:
	Asteroids(float timeLeft, float lastFrameTicks, bool finished);
	GLuint LoadTexture(const char* img);
	float generateRandomFloat(float lowerBound, float upperBound);
	bool updateAndRender();
	//void Render();
	void FixedUpdate();
	void keyBoardControls(float elapsed);
	//void collisions(sprite* s, sprite* s2, float distanceX, float distanceY);
	void calculateAdjustments(sprite* e, sprite* e2, float distanceX, float distanceY);
	bool collisions(sprite* s, sprite* s2, float distanceX, float distanceY, bool flag);
	void menu();
	void drawText(GLuint texture, string text, float x, float y, float s, float r, float g, float b, float a);
};