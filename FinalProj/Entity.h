//
//  Entity.h
//  HW6
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard on 4/21/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//

#pragma once
#include <stdio.h>

#include "Matrix.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

float lerp(float v0, float v1, float t);

class Color {
public:
    Color();
    Color(float r, float g, float b, float a);
    float r;
    float g;
    float b;
    float a;
};
class Particle {
public:
    Vector position;
    Vector velocity;
    float lifetime;
};

class ParticleEmitter {
public:
    
    ParticleEmitter(unsigned int particleCount);
    ParticleEmitter();
    ~ParticleEmitter();
    void Update(float elapsed);
    void Render();
    Vector position;
    Vector gravity;
    Vector velocity;
    Vector velocityDeviation;
    float maxLifetime;
    Color startColor;
    Color endColor;
    vector<Particle> particles;
};

class SheetSprite{
    
public:
    SheetSprite();
    
    SheetSprite(unsigned int texID, float u, float v, float w, float h);
    SheetSprite(unsigned int texID, int countX, int countY);
    
    unsigned int textureID;
    
    int countX;
    int countY;
    
    float u;
    float v;
    float width;
    float height;
};

class Entity{
public:
    
    Entity();
    
    Entity(SheetSprite* sprite, float x, float y, float sx, float sy, float r);
    Entity(unsigned int tex, float u, float v, float w, float h, float x = 0, float y = 0, float sx = 1, float sy = 1, float r = 0);
    
    ~Entity();
    void Draw(GLfloat coordinates[]);
    void Build();
    void Draw();
    void BuildUniform(int index);
    void DrawUniform(int index);
    
    void buildMatrix();
    
    Vector position;
  
    string color;

    Vector scale;

    float rotation;
    
    Matrix matrix;
    
    bool visible;
    
    Vector velocity;
    Vector acceleration;
    
    bool angry;
    float speed;

    bool dead;
    bool collidedTop;
    bool collidedBot;
    bool collidedLeft;
    bool collidedRight;
    
    vector<Entity*> hearts;
    int playerCoins;
    vector<Entity*> playerKeys;
    vector<Entity*> bullets;
    
    bool flipped;
    float lifeTime;
    
    float anim_speed;
    SheetSprite* sprite;
    
};
