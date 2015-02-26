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
#include <algorithm>
#include <string>
#include <vector>
#include "sprite.h"

using namespace std;

enum state {MAIN_MENU, PLAY_GAME, OVER};

//class Sprite;

class sInvaders{
public:
	sInvaders();
	void initialize();
	void setup(); 
	void menu(); 
	GLuint photoTexture(const char* img); 
	void process();
	void playerShoot(Sprite& player);
	void enemyShoot(Sprite& enemy); 
	void drawText(GLuint texture, string text, float x, float y, float s, float r, float g, float b, float a); //done
	void game();
	void render(); 
	void playGame();
	bool update(); 
	void end();
private:
	int state, ndx, score;
	float playerWait, enemyWait, timeElapsed, gameClock;
	vector<Sprite> player;
	vector<Sprite> pAmmo;
	vector<Sprite> eAmmo;
	vector<Sprite> enemy;
	GLuint size;
	const Uint8* key;
	SDL_Event event;
	SDL_Window* window;
	bool finished;
};