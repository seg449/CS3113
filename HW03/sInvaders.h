/*
Sebastien Emmanuel Gerard
CS3113
HW03
2/26/15
*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
//#include <algorithm>
#include <string>
#include <vector>
#include "sprite.h"

using namespace std;

enum state {MAIN_MENU, PLAY_GAME, OVER};

//class Sprite;

class sInvaders{
public:
	sInvaders(float playerTime, float enemyTime, int score, bool finished);
	GLuint photoTexture(const char* img); 
	void keyboardInput();
	void drawText(GLuint texture, string text, float x, float y, float s, float r, float g, float b, float a); 
	void render(); 
	void checkCollisions();
	bool playGame(); 
	Sprite player;
	int count;

private:
	int state, ndx, score, index2;
	float playerWait, enemyWait, timeElapsed, gameClock;
	vector<Sprite> pAmmo;
	vector<Sprite> eAmmo;
	vector<Sprite> enemy;
	GLuint size;
	SDL_Event event;
	SDL_Window* window;
	bool finished;
};