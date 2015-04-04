#pragma once

#include "Sprite.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

enum state {MAIN_MENU, GAME, OVER};

class platformers {
	vector<Sprite*> regObjects;
	vector<Sprite*> bombs;
	bool finished, isVisible;
	float timeElapsed, lfTicks, score; 
	int state, mapWidth, mapHeight;
	GLuint sheet, size;
	SDL_Window* window;
	Sprite* player;
	Mix_Chunk* coinS;
	Mix_Chunk* bombSound;
	Mix_Music* music;
	unsigned char** levelData;

public:
	platformers(int score, bool finished, float lfTicks, float timeElapsed);
	GLuint photoTexture(const char* img);
	void processEvents(float time);
	void menu();
	void drawText(GLuint texture, string text, float x, float y, float s, float r, float g, float b, float a); 
	bool updateAndRender();
	void keyboardControl(float elapsed);
	void FixedUpdate();
	SDL_Event event;
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);
	void placeEntity(string type, float placeX, float placeY);
	void renderMap();
	bool leftSpriteCollision(Sprite* s, float* newVal);
	bool rightSpriteCollision(Sprite* s, float* newVal); 
	bool topSpriteCollision(Sprite* s, float* newVal);
	bool bottomSpriteCollision(Sprite* s, float* newVal); 
};