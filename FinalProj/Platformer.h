//
//  Platformer.h
//  Platformer
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard on 4/28/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//

#pragma once


#pragma once
#include "Entity.h"
#include "Perlin.h"

#define MAX_BULLETS 80
#define FIXED_TIMESTEP 0.01555555f
#define MAX_TIMESTEPS 10
#define TILE_SIZE 0.33f


enum GameState { MAIN_MENU, LEVEL_ONE, LEVEL_TWO, LEVEL_THREE, LEVEL_FOUR, GAME_OVER };

unsigned int LoadTexture(const char *image_path);

void DrawText(int fontTex, string text, float x, float y, float size, float spacing, float r, float g, float b, float a);

void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);

class Platformer{
public:
    Platformer();
    ~Platformer();
    void shoot(Entity* shooter, Entity* bullets[], int& index);
    
    bool checkCollision(Entity* a, Entity* b);
    
    void DrawText(int fontTex, std::string text, float x, float y, float size, float spacing, float r, float g, float b, float a);
    
    void readLevelOne();
    void readLevelTwo();
    void readLevelThree();
    void readLevelFour();
    
    void setup();
    bool readLayerData(ifstream &stream);
    bool readHeader(ifstream &stream);
    bool readEntityData(ifstream &stream);
    bool readCollisionData(ifstream &stream);
    
    void placeEntity(string type, float placeX, float placeY);
    void placeBlock(float placeX, float placeY);
    
    void playerWalk(int anim_index);
    
    void shoot(Entity* shooter, vector<Entity*> bullets, int& index);
    
    bool UpdateAndRender();
    
    void checkYCollisions(Entity* entity);
    void checkXCollisions(Entity* entity);
    
    void checkBotCollision(Entity* entity);
    void checkLeftCollision(Entity* entity);
    void checkTopCollision(Entity* entity);
    void checkRightCollision(Entity* entity);
    
    void Render();
    void Update(float elapsed);
    void resetGame();
    
    void UpdateMainMenu(float elapsed);
    void RenderMainMenu();
    
    void UpdateGameLevel(float elapsed);
    void RenderGameLevel();
    
    void UpdateGameOver();
    void RenderGameOver();
    
    void fixedUpdate();
    
    void BuildLevel(unsigned int texture);
    
private:
    int state, mapWidth, mapHeight, tile_spriteX, tile_spriteY, player_spriteX, player_spriteY;
    unsigned int** levelData;
    unsigned int font_tex, sprite_sheet, sprite_sheet2, items, tiles, hud, enemy;
    bool done;
    float lastFrameTicks;
    float timeLeftOver;
    
    int anim_index;
    int runAnimation[11];
    int numFrames;
    float animationElapsed;
    float fly_animationElapsed;
    float fly_framesPerSecond;
    float perlinValue;
    float animationTime;
    float animationValue;
    float framesPerSecond;
    
    Mix_Music* music;
    Mix_Chunk* jump;
    Mix_Chunk* coin;
    
    SDL_Window* window;

    
    float friction;
    float gravity;
    
    Entity* bop;
    Entity* coinHud;
    vector<Entity*> keyHuds;
    
 //   ParticleEmitter par;
    
    int bIndex;
    
    Entity* player;
    Entity* door;
    vector<int> collideTiles;
    vector<int> harmTiles;
    vector<Entity*> clouds;
    vector<Entity*> blocks;
    vector<Entity*> flies;
    vector<Entity*> slimes;
    vector<Entity*> coins;
    vector<Entity*> stars;
    vector<Entity*> keys;
    vector<Entity*> buttons;
    vector<Entity*> powerups;
    //Entity* enemies[20];

};
