#pragma once

#include "Sprite.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

enum state {MAIN_MENU, GAME, OVER};

class platformers {
	vector<Sprite*> solidObjects;
	vector<Sprite*> regObjects;
	bool finished, isVisible;
	float timeElapsed, lfTicks, score; 
	int state;
	GLuint sheet, size;
	SDL_Window* window;
	Sprite* player;

public:
	platformers(int score, bool finished, float lfTicks, float timeElapsed);
	void processEvents(float time);
	void menu();
	void drawText(GLuint texture, string text, float x, float y, float s, float r, float g, float b, float a); 
	GLuint photoTexture(const char* img);
	bool updateAndRender();
	void keyboardControl(float elapsed);
	void renderObjects();
	void FixedUpdate();
	SDL_Event event;
};