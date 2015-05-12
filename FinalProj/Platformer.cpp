//
//  Platformer.cpp
//  Platformer
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard 4/28/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//


#include "Platformer.h"


float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
    float retVal = dstMin + ((value - srcMin)/(srcMax-srcMin) * (dstMax-dstMin));
    if(retVal < dstMin) {
        retVal = dstMin;
    }
    if(retVal > dstMax) {
        retVal = dstMax;
    }
    return retVal;
}
float easeOut(float from, float to, float time) {
    float oneMinusT = 1.0f-time;
    float tVal =  1.0f - (oneMinusT * oneMinusT * oneMinusT *
                          oneMinusT * oneMinusT);
    return (1.0f-tVal)*from + tVal*to;
}
float easeInOut(float from, float to, float time) {
    float tVal;
    if(time > 0.5) {
        float oneMinusT = 1.0f-((0.5f-time)*-2.0f);
        tVal =  1.0f - ((oneMinusT * oneMinusT * oneMinusT * oneMinusT *
                         oneMinusT) * 0.5f);
    } else {
        time *= 2.0;
        tVal = (time*time*time*time*time)/2.0;
    }
    return (1.0f-tVal)*from + tVal*to;
}
float easeOutElastic(float from, float to, float time) {
    float p = 0.3f;
    float s = p/4.0f;
    float diff = (to - from);
    return from + diff + (diff*pow(2.0f,-10.0f*time) * sin((time-s)*(2*M_PI)/p));
}
// taken from slides
// loads texture from an image path
unsigned int LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        exit(1);
        // handle error
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    SDL_FreeSurface(surface);
    return tex;
}
void Platformer::DrawText(int fontTex, std::string text, float x, float y, float size, float spacing, float r, float g, float b, float a) {
    
    glBindTexture(GL_TEXTURE_2D, fontTex);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    float texture_size = 1.0/16.0f;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    
    for (int i = 0; i < text.size(); i++) {
        
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        
        vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size});
        
        texCoordData.insert(texCoordData.end(), {texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
            texture_size, texture_y + texture_size, texture_x + texture_size, texture_y});
    }
    if (state == GAME_OVER) glLoadIdentity();
    glTranslatef(x, y, 0);
    
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


Platformer::Platformer() {
    setup();
    done = false;
    lastFrameTicks = 0.0f;
}
Platformer::~Platformer(){
    
    for (Entity* e : slimes) {
        delete e;
    }
    for (Entity* e : flies) {
        delete e;
    }
    delete player;
    
    //   Mix_FreeChunk(someSound);
    Mix_FreeMusic(music);

    
    SDL_Quit();
}

void Platformer::readLevelOne() {
    ifstream infile("final_level1.txt");
    if (!infile) cerr << "err" << endl;
    string line;
    while (getline(infile, line)){
        
        if (line == "[header]"){
            if (!readHeader(infile)) return;
        }
        else if (line == "[layer]"){
            readLayerData(infile);
        }
        else if (line == "[Items]"){
            readEntityData(infile);
        }
    }

}
void Platformer::readLevelTwo() {
    ifstream infile("final_level2.txt");
    if (!infile) cerr << "err" << endl;
    string line;
    while (getline(infile, line)){
        
        if (line == "[header]"){
            if (!readHeader(infile)) return;
        }
        else if (line == "[layer]"){
            readLayerData(infile);
        }
        else if (line == "[Objects]"){
            readEntityData(infile);
        }
    }
}
void Platformer::readLevelThree() {
    ifstream infile("final_level3.txt");
    if (!infile) cerr << "err" << endl;
    string line;
    while (getline(infile, line)){
        
        if (line == "[header]"){
            if (!readHeader(infile)) return;
        }
        else if (line == "[layer]"){
            readLayerData(infile);
        }
        else if (line == "[Objects]"){
            readEntityData(infile);
        }
    }
}
void Platformer::readLevelFour() {
    ifstream infile("final_level4.txt");
    if (!infile) cerr << "err" << endl;
    string line;
    while (getline(infile, line)){
        
        if (line == "[header]"){
            if (!readHeader(infile)) return;
        }
        else if (line == "[layer]"){
            readLayerData(infile);
        }
        else if (line == "[Objects]"){
            readEntityData(infile);
        }
    }
}

void Platformer::setup() {
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    
    music = Mix_LoadMUS("music.mp3");
    coin = Mix_LoadWAV("coin.wav");
    jump = Mix_LoadWAV("jump.wav");
    
    Mix_PlayMusic(music, -1);
    
    glViewport(0,0,1200,800);
    
    glMatrixMode(GL_PROJECTION);
 //   glOrtho(-2.66, 2.66, -2.0, 2.0, -2.0, 2.0);
 //   glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    
    glOrtho(-2.9, 2.9, -0.9, 1.7, -0.9, 1.7);
 //   glOrtho(-2.7, 2.7, -0.8, 1.7, -0.8, 1.7);
    
    // gray
//    glClearColor(0.3f,0.3f,0.3f,0.0f);
    
    // sky blue
    glClearColor(0.150, 0.55, 0.65, 0.00);
    
    bIndex = 0;
    anim_index = 0;
    numFrames = 7;
    animationElapsed = 0.0f;
    framesPerSecond = 50.0f;
    perlinValue = 0;
    animationTime = 0;
    fly_animationElapsed = 0.0f;
    fly_framesPerSecond = 10.0f;
    
    for (int i = 0; i < 7; i++) {
        runAnimation[i] = i;
    }
    
    
    font_tex = LoadTexture("font.png");
    sprite_sheet = LoadTexture("tiles_spritesheet.png");
    sprite_sheet2 = LoadTexture("p1_walk.png");
    items = LoadTexture("items.png");
    tiles = LoadTexture("tiles.png");
    hud = LoadTexture("hud.png");
    enemy = LoadTexture("enemies.png");
    
 //   unsigned int test = LoadTexture("bg.png");
   
    
    
    tile_spriteX = 12;
    tile_spriteY = 12;
    //    player_spriteX = 3;
    //    player_spriteY = 4;
    
    //    float u = (float)(112 % player_spriteX) / player_spriteX;
    //    float v = (float)(112 / player_spriteX) / player_spriteY;
    //    float width = 1.0f / player_spriteX;
    //    float height = 1.0f / player_spriteY;
    //
    SheetSprite* p1 = new SheetSprite(sprite_sheet2, 11, 1);
    player = new Entity(p1, 2.685, -6, 0.95, 0.2, 0);
    player->visible = true;
    player->sprite->width -= 0.0045;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->acceleration.x = 0;
    player->acceleration.y = 0;
    player->speed = 1.5;
    
    player->playerCoins = 0;
    
//    Entity* enemy = new Entity(enemies, 0.0/353.0, 32.0/153.0, 72.0/353.0, 36.0/353.0,
    
//    bop = new Entity(test, 0, 0, 1, 1, player->position.x, player->position.y, 0.02, 0.02, 0);
//    bop->visible = true;
//    bop->velocity.x = 0;
//    bop->velocity.y = 0;
//    bop->acceleration.x = 0;
//    bop->acceleration.y = 0;
    
    Entity* heart1 = new Entity(hud, 0.0/269.0, 94.0/269.0, 53.0/269.0, 45.0/269.0, 2.0, 1.4, 0.3, 0.3, 0);
    heart1->visible = true;
    heart1->velocity.x = 0;
    heart1->velocity.y = 0;
    heart1->acceleration.x = 0;
    heart1->acceleration.y = 0;
    heart1->color = "Full";
    Entity* heart2= new Entity(hud, 0.0/269.0, 94.0/269.0, 53.0/269.0, 45.0/269.0, 2.2, 1.4, 0.3, 0.3, 0);
    heart2->visible = true;
    heart2->velocity.x = 0;
    heart2->velocity.y = 0;
    heart2->acceleration.x = 0;
    heart2->acceleration.y = 0;
      heart2->color = "Full";
    Entity* heart3 = new Entity(hud, 0.0/269.0, 94.0/269.0, 53.0/269.0, 45.0/269.0, 2.4, 1.4, 0.3, 0.3, 0);
    heart3->visible = true;
    heart3->velocity.x = 0;
    heart3->velocity.y = 0;
    heart3->acceleration.x = 0;
    heart3->acceleration.y = 0;
      heart3->color = "Full";
    Entity* heart4 = new Entity(hud, 0.0/269.0, 94.0/269.0, 53.0/269.0, 45.0/269.0, 2.4, 1.4, 0.3, 0.3, 0);
    heart4->visible = true;
    heart4->velocity.x = 0;
    heart4->velocity.y = 0;
    heart4->acceleration.x = 0;
    heart4->acceleration.y = 0;
      heart4->color = "Full";
    player->hearts = {heart1, heart2, heart3, heart4};
    //reverse(player->hearts.begin(), player->hearts.end());
    
    coinHud = new Entity(hud, 55.0/269.0, 0.0/269.0, 47.0/269.0, 47.0/269.0, 2.6, 1.6, 0.2, 0.2, 0);
    coinHud->visible = true;
    coinHud->velocity.x = 0;
    coinHud->velocity.y = 0;
    coinHud->acceleration.x = 0;
    coinHud->acceleration.y = 0;
    
    Entity* key1 = new Entity(hud, 150.0/269.0, 38.0/269.0, 44.0/269.0, 40.0/269.0);
    key1->velocity.x = 0;
    key1->velocity.y = 0;
    key1->acceleration.x = 0;
    key1->acceleration.y = 0;
    key1->color = "Blue";
    key1->visible = false;
    Entity* key2 = new Entity(hud, 104.0/269.0, 38.0/269.0, 44.0/269.0, 40.0/269.0);
    key2->velocity.x = 0;
    key2->velocity.y = 0;
    key2->acceleration.x = 0;
    key2->acceleration.y = 0;
    key2->color = "Green";
    key2->visible = false;
    Entity* key3 = new Entity(hud, 192.0/269.0, 164.0/269.0, 44.0/269.0, 40.0/269.0);
    key3->velocity.x = 0;
    key3->velocity.y = 0;
    key3->acceleration.x = 0;
    key3->acceleration.y = 0;
    key3->color = "Orange";
    key3->visible = false;
    Entity* key4 = new Entity(hud, 147.0/269.0, 80.0/269.0, 44.0/269.0, 40.0/269.0);
    key4->velocity.x = 0;
    key4->velocity.y = 0;
    key4->acceleration.x = 0;
    key4->acceleration.y = 0;
    key4->color = "Yellow";
    key4->visible = false;
    keyHuds = {key1, key2, key3, key4};
    

    
    
//    par = ParticleEmitter(10);
    
    
//
//     Entity* heartHalf = new Entity(hud, 0.0/269.0, 0.0/269.0, 53.0/269.0, 45.0/269.0, 2.3, 1.4, 1, 1, 0);
//    
//      Entity* heartEmpty = new Entity(hud, 0.0/269.0, 47.0/269.0, 53.0/269.0, 45.0/269.0, 2.5, 1.4, 1, 1, 0);
    
    
    state = MAIN_MENU;
    done = false;
    lastFrameTicks = 0.0f;
    
    gravity = 15;
    friction = 8;
    
    collideTiles = {42, 45, 67, 8, 56, 10, 11, 22, 43, 31, 19, 21, 117, 93, 115, 116, 127, 138};
    harmTiles = {46, 35, 60};

    readLevelOne();
    
//    ifstream infile("final_level1.txt");
//    if (!infile) cerr << "err" << endl;
//    string line;
//    while (getline(infile, line)){
//        
//        if (line == "[header]"){
//            if (!readHeader(infile)) return;
//        }
//        else if (line == "[layer]"){
//            readLayerData(infile);
//        }
//        //        else if (line == "[Collision]") {
//        //            readCollisionData(infile);
//        //
//        //        }
//        else if (line == "[Items]"){
//            readEntityData(infile);
//        }
//    }
    
    for (Entity* k_hud : keyHuds) {
        for (Entity* key : keys) {
            if (key->color == k_hud->color) k_hud->visible = true;
        }
    }
}

void Platformer::placeEntity(string type, float placeX, float placeY){
    if (type == "Coin"){
        
        
        Entity* coin = new Entity(items, 288.0/576.0, 360.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
        coin->visible = true;
        coin->velocity.x = 0;
        coin->velocity.y = 0;
        coin->acceleration.x = 0;
        coin->acceleration.y = 0;
        coins.push_back(coin);
    }
    
        else if (type == "Cloud1"){
            
            
            Entity* cloud = new Entity(items, 0.0/576.0, 146.0/576.0, 128.0/576.0, 71.0/576.0, placeX, placeY);
            cloud->visible = true;
            cloud->velocity.x = 0;
            cloud->velocity.y = 0;
            cloud->acceleration.x = 0;
            cloud->acceleration.y = 0;
            clouds.push_back(cloud);
        }
//    if (type == "Cloud2"){
//        
//        
//        Entity* coin = new Entity(items, 288.0/576.0, 360.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
//        coin->visible = true;
//        coin->velocity.x = 0;
//        coin->velocity.y = 0;
//        coin->acceleration.x = 0;
//        coin->acceleration.y = 0;
//        coins.push_back(coin);
//    }

    
//    else if (type == "Enemy1"){
//        
//        Entity* e = new Entity(enemy, 52.0/353.0, 125.0/153.0, 50.0/353.0, 28.0/153.0, placeX, placeY);
//        e->visible = true;
//        e->velocity.x = 0;
//        e->velocity.y = 0;
//        e->acceleration.x = 0;
//        e->acceleration.y = 0;
//        e->scale.x = 0.7;
//        e->scale.y = 0.4;
//        slimes.push_back(e);
//    }
    else if (type == "Enemy2"){
        
        Entity* e = new Entity(enemy, 0.0/353.0, 32.0/153.0, 72.0/353.0, 36.0/153.0, placeX, placeY);
        e->visible = true;
        e->angry = false;
        e->dead = false;
        e->velocity.x = 0;
        e->velocity.y = 0;
        e->acceleration.x = 0;
        e->acceleration.y = 0;
        e->scale.x = 0.6;
        e->scale.y = 0.4;
        flies.push_back(e);
    }
    else if (type == "Button"){
        
        
        Entity* button = new Entity(items, 360.0/576.0, 360.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
        button->visible = true;
        button->velocity.x = 0;
        button->velocity.y = 0;
        button->acceleration.x = 0;
        button->acceleration.y = 0;
        buttons.push_back(button);
        
////        float u = (float)(426 % 64) / 64.0f;
////        float v = (float)(426 / 64) / 32.0f;
////        float width = 1.0f / 64.0f;
////        float height = 1.0f / 32.0f;
////        
////        Entity* enemy = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
////        enemy->scale = 2;
////        enemy->acceleration_y = 0.2;
////        enemies.push_back(enemy);
//        float u = (float)(1023 % 43) / (float)43.0;
//        float v = (float)(1023 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* button = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//   
//        buttons.push_back(button);
        
    }
    else if (type == "Star"){
        
        Entity* star = new Entity(items, 504.0/576.0, 288.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
        star->visible = true;
        star->velocity.x = 0;
        star->velocity.y = 0;
        star->acceleration.x = 0;
        star->acceleration.y = 0;
        stars.push_back(star);
        
//        float u = (float)(319 % 43) / (float)43.0;
//        float v = (float)(319 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* star = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//      
//       // yellows.push_back(yellow);
    }
    else if (type == "BlueKey"){
        
        Entity* key = new Entity(items, 131.0/576.0, 0.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
        key->visible = true;
        key->color = "Blue";
        key->velocity.x = 0;
        key->velocity.y = 0;
        key->acceleration.x = 0;
        key->acceleration.y = 0;
        keys.push_back(key);
//        float u = (float)(454 % 43) / (float)43.0;
//        float v = (float)(454 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* key = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        //greens.push_back(green);
    }
    else if (type == "OrangeKey"){
        
        Entity* key = new Entity(items, 72.0/576.0, 435.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
        key->visible = true;
        key->color = "Orange";
        key->velocity.x = 0;
        key->velocity.y = 0;
        key->acceleration.x = 0;
        key->acceleration.y = 0;
        keys.push_back(key);
//        float u = (float)(454 % 43) / (float)43.0;
//        float v = (float)(454 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* key = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        //greens.push_back(green);
    }
    else if (type == "YellowKey"){
        
        Entity* key = new Entity(items, 72.0/576.0, 363.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
        key->visible = true;
        key->color = "Yellow";
        key->velocity.x = 0;
        key->velocity.y = 0;
        key->acceleration.x = 0;
        key->acceleration.y = 0;
        keys.push_back(key);
//        float u = (float)(454 % 43) / (float)43.0;
//        float v = (float)(454 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* key = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        //greens.push_back(green);
    }
    else if (type == "GreenKey"){
        
        Entity* key = new Entity(items, 130.0/576.0, 146.0/576.0, 70.0/576.0, 70.0/576.0, placeX, placeY);
        key->visible = true;
        key->color = "Green";
        key->velocity.x = 0;
        key->velocity.y = 0;
        key->acceleration.x = 0;
        key->acceleration.y = 0;
        keys.push_back(key);
//        float u = (float)(454 % 43) / (float)43.0;
//        float v = (float)(454 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* key = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        //greens.push_back(green);
    }
    else if (type == "BluePowerup"){
        
        Entity* powerup = new Entity(tiles, 432.0/914.0, 504.0/936.0, 70.0/914.0, 70.0/936.0, placeX, placeY);
        powerup->visible = true;
        powerup->color = "Blue";
        powerup->velocity.x = 0;
        powerup->velocity.y = 0;
        powerup->acceleration.x = 0;
        powerup->acceleration.y = 0;
        powerup->scale.x = 1.5;
        powerup->scale.y = 1.5;
        powerups.push_back(powerup);
//        float u = (float)(319 % 43) / (float)43.0;
//        float v = (float)(319 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* powerup = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        // yellows.push_back(yellow);
    }
    else if (type == "OrangePowerup"){
        
        Entity* powerup = new Entity(tiles, 432.0/914.0, 360.0/936.0, 70.0/914.0, 70.0/936.0, placeX, placeY);
        powerup->visible = true;
        powerup->color = "Orange";
        powerup->velocity.x = 0;
        powerup->velocity.y = 0;
        powerup->acceleration.x = 0;
        powerup->acceleration.y = 0;
        powerup->scale.x = 1.5;
        powerup->scale.y = 1.5;
        powerups.push_back(powerup);
        
//        float u = (float)(319 % 43) / (float)43.0;
//        float v = (float)(319 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* powerup = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        // yellows.push_back(yellow);
    }
    else if (type == "YellowPowerup"){
        
        Entity* powerup = new Entity(tiles, 432.0/914.0, 288.0/936.0, 70.0/914.0, 70.0/936.0, placeX, placeY);
        powerup->visible = true;
        powerup->color = "Yellow";
        powerup->velocity.x = 0;
        powerup->velocity.y = 0;
        powerup->acceleration.x = 0;
        powerup->acceleration.y = 0;
        powerup->scale.x = 1.5;
        powerup->scale.y = 1.5;
        powerups.push_back(powerup);
        
//        float u = (float)(319 % 43) / (float)43.0;
//        float v = (float)(319 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* powerup = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        // yellows.push_back(yellow);
    }
    else if (type == "GreenPowerup"){
        
        Entity* powerup = new Entity(tiles, 72.0/914.0, 576.0/936.0, 70.0/914.0, 70.0/936.0, placeX, placeY);
        powerup->visible = true;
        powerup->color = "Green";
        powerup->velocity.x = 0;
        powerup->velocity.y = 0;
        powerup->acceleration.x = 0;
        powerup->acceleration.y = 0;
        powerup->scale.x = 1.5;
        powerup->scale.y = 1.5;
        powerups.push_back(powerup);
        
//        float u = (float)(319 % 43) / (float)43.0;
//        float v = (float)(319 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* powerup = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        // yellows.push_back(yellow);
    }
    else if (type == "Block"){
        
        Entity* block = new Entity(tiles, 0.0/914.0, 144.0/936.0, 70.0/914.0, 70.0/936.0, placeX, placeY);
        block->visible = true;
        block->velocity.x = 0;
        block->velocity.y = 0;
        block->acceleration.x = 0;
        block->acceleration.y = 0;
        block->scale.x = 1.5;
        block->scale.y = 1.5;
//        block->scale = block->matrix.inverse() * block->scale;
//        block->position.y += 0.05;
        blocks.push_back(block);
//        float u = (float)(319 % 43) / (float)43.0;
//        float v = (float)(319 / 43) / (float)43.0;
//        float width = 1.0f;
//        float height = 1.0f;
//        
//        Entity* block = new Entity(sprite_sheet, u, v, width, height, placeX, placeY);
//        
//        // yellows.push_back(yellow);
    }
//    else if (type == "Door"){
//    
//        door = new Entity(tiles, 648.0/914.0, 432.0/936.0, 70.0/914.0, 70.0/936.0, placeX, placeY);
//        door->visible = true;
//        door->velocity.x = 0;
//        door->velocity.y = 0;
//        door->acceleration.x = 0;
//        door->acceleration.y = 0;
//        
//        // yellows.push_back(yellow);
//    }
}
bool Platformer::readHeader(ifstream &stream) {
    string line;
    mapWidth = -1;
    mapHeight = -1;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "width") {
            mapWidth = atoi(value.c_str());
        } else if(key == "height"){
            mapHeight = atoi(value.c_str());
        }
    }
    if(mapWidth == -1 || mapHeight == -1) {
        return false;
    } else { // allocate our map data
        levelData = new unsigned int*[mapHeight];
        for(int i = 0; i < mapHeight; ++i) {
            levelData[i] = new unsigned int[mapWidth];
        }
        return true;
    }
}
bool Platformer::readLayerData(ifstream &stream) {
    string line;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
     

        if(key == "data") {
            for(int y=0; y < mapHeight; y++) {
                getline(stream, line);
                istringstream lineStream(line);
                string tile;
                for(int x=0; x < mapWidth; x++) {
                    getline(lineStream, tile, ',');
                    unsigned int val = stoi(tile);
                    if(val > 0) {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        int index = val-1;
                        levelData[y][x] = index;
//                        if (index != 35 && index != 2 && index != 77 && index != 6 && index != 17 && index != 29 && index != 128 && index != 141) {
//                            tiles.push_back(index);
//                        }
                    } else {
                        levelData[y][x] = 0;
                    }
                }
            }
        }
 //   }
    }
//    for (int i : tiles) cout << i << endl;
    return true;
}
bool Platformer::readEntityData(ifstream &stream) {
    string line;
    string type;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "type") {
            type = value;
        } else if(key == "location") {
            istringstream lineStream(value);
            string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            float placeX = stof(xPosition)*TILE_SIZE;
            float placeY = stof(yPosition)*-TILE_SIZE + 0.12;
            
            placeEntity(type, placeX, placeY);
        }
    }
    return true;
}


void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
    *gridX = (int)(worldX / TILE_SIZE);
    *gridY = (int)(-worldY / TILE_SIZE);
}


void Platformer::resetGame() {
    
    flies.clear();
    player->playerCoins = 0;
    player->playerKeys.clear();
    for (Entity* heart : player->hearts) {
        heart->sprite->u = 0.0/269.0;
        heart->sprite->v = 94.0/269.0;
        heart->color = "Full";
        
    }
    player->position.x = 2.7;
    player->position.y = -6;
    player->visible = true;
    player->sprite->width -= 0.0045;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->acceleration.x = 0;
    player->acceleration.y = 0;
    player->speed = 1.5;
    readLevelOne();
    done = false;
}

void Platformer::BuildLevel(unsigned int texture) {
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    vector<float> vertexData;
    vector<float> texCoordData;

    
    for (int y=0; y < mapHeight; y++) {
        for(int x=0; x < mapWidth; x++) {

            if (levelData[y][x] != 0) {

                float u = (float)(((int)levelData[y][x]) % tile_spriteX) / (float) tile_spriteX;
                float v = (float)(((int)levelData[y][x]) / tile_spriteX) / (float) tile_spriteY;
                float spriteWidth = 1.0/tile_spriteX - 0.002;
                float spriteHeight = 1.0/tile_spriteY - 0.0039;
                
                vertexData.insert(vertexData.end(), {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
                });
                texCoordData.insert(texCoordData.end(), { u, v,
                    u, v+(spriteHeight),
                    u+spriteWidth, v+(spriteHeight),
                    u+spriteWidth, v
                });
            }
            else {
                float u = 0;
                float v = 0;
                float spriteWidth = 0;
                float spriteHeight = 0;
                
                vertexData.insert(vertexData.end(), {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
                });
                texCoordData.insert(texCoordData.end(), { u, v,
                    u, v+(spriteHeight),
                    u+spriteWidth, v+(spriteHeight),
                    u+spriteWidth, v
                });
            }
        }
    }

    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, (mapHeight * mapWidth) * 4.0);
    glDisable(GL_TEXTURE_2D);
}
void Platformer::checkBotCollision(Entity* entity) {
    int gridX, gridY;
    float val = entity->position.y - (entity->sprite->height * entity->scale.y * entity->scale.x);

//    float val = (player->position.y - (player->sprite->height / 2));
    worldToTileCoordinates(entity->position.x, val, &gridX, &gridY);
    
    entity->collidedBot = false;
    
    if (levelData[gridY][gridX] == 65) {
        if (state == LEVEL_ONE) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelTwo();
            state = LEVEL_TWO;
        }
        else if (state == LEVEL_TWO) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelThree();
            state = LEVEL_THREE;
        }
        else if (state == LEVEL_THREE){
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelFour();
           state = LEVEL_FOUR;
        }
        else {
            state = GAME_OVER;
        }
    }

    if (gridX > 0 && gridY > 0){
        
        for (int i : collideTiles) {
            if (levelData[gridY][gridX] == i) {
                float y_dist = fabs(val - (-(gridY*TILE_SIZE)));
                
                entity->position.y += y_dist + 0.0001;
                entity->velocity.y = 0;
                entity->collidedBot = true;
            }
        }
        
        for (int i : harmTiles) {
            if (levelData[gridY][gridX] == i) {
            //    float y_dist = fabs(val - (-(gridY*TILE_SIZE)));
                if (entity->flipped){
                entity->position.x += 0.8;
                entity->position.y += 0.3;
                entity->velocity.y = 0;
                }
                else{
                    entity->position.x -= 0.8;
                    entity->position.y += 0.3;
                    entity->velocity.y = 0;
                }
                for (auto it = entity->hearts.rbegin(); it != entity->hearts.rend(); ++it) {
                
//                for (Entity* heart : player->hearts) {
                
              
                    if ((*it)->color == "Empty") continue;
                    
                    
                    else if ((*it)->color == "Full"){
                        (*it)->sprite->v = 0.0;
                        (*it)->color = "Half";
                       
                        break;
                    }
                    else if ((*it)->color == "Half") {
                        (*it)->sprite->v = 47.0/269.0;
                        (*it)->color = "Empty";
                    
                        break;

                    }
                    
                }
            }
        }
    }
}
void Platformer::checkTopCollision(Entity* entity) {
    
    int gridX, gridY;
//    float val = player->position.y + ((player->sprite->height / 4) * player->scale.y * player->scale.x);
    float val = entity->position.y + (entity->sprite->height/4 * entity->scale.y * entity->scale.x);


//    float val = player->position.y;

    worldToTileCoordinates(entity->position.x, val, &gridX, &gridY);
    
    entity->collidedTop = false;
    
    for (int i : collideTiles) {
        if (levelData[gridY][gridX] == i){
            
            val += 0.35;
            
            float y_dist = (fabs(val - (-(gridY*TILE_SIZE))));
            entity->position.y -= y_dist + 0.0001;
            entity->velocity.y = 0;
            entity->collidedTop = true;
        }
    }
    if (levelData[gridY][gridX] == 65) {
        if (state == LEVEL_ONE) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelTwo();
            state = LEVEL_TWO;
        }
        else if (state == LEVEL_TWO) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelThree();
            state = LEVEL_THREE;
        }
        else if (state == LEVEL_THREE){
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelFour();
            state = LEVEL_FOUR;
        }
        else {
            state = GAME_OVER;
        }
    }


}
void Platformer::checkLeftCollision(Entity* entity) {
    
    int gridX, gridY;
    float val = entity->position.x - entity->sprite->width / 2;
//    float val = player->position.x + (player->sprite->width * player->scale.x);
//    float val = player->position.x - (player->sprite->width / 2);
    
//    float val = player->position.y - (player->sprite->height * player->scale.y);


//    worldToTileCoordinates(val, player->position.y, &gridX, &gridY);
    worldToTileCoordinates(val, entity->position.y, &gridX, &gridY);
    
    entity->collidedLeft = false;
    
    for (int i : collideTiles) {
        if (levelData[gridY][gridX] == i){
            
            val -= 0.3;
 //           float x_dist = fabs(val - ((gridX*TILE_SIZE)));
            
            float x_dist = fabs(val - ((gridX*TILE_SIZE)));
            
            entity->position.x += x_dist + 0.001;
            entity->velocity.x = 0;
            entity->collidedLeft = true;
        }
    }
    if (levelData[gridY][gridX] == 65) {
        if (state == LEVEL_ONE) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelTwo();
            state = LEVEL_TWO;
        }
        else if (state == LEVEL_TWO) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelThree();
            state = LEVEL_THREE;
        }
        else if (state == LEVEL_THREE){
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelFour();
            state = LEVEL_FOUR;
        }
        else {
            state = GAME_OVER;
        }
    }


//    int gridX, gridY;
//    float val = player->position.x - (player->sprite->width / 2);
//    worldToTileCoordinates(val, player->position.y, &gridX, &gridY);
//    
//    if (levelData[gridY][gridX] == 98 || levelData[gridY][gridX] == 161 || levelData[gridY][gridX] == 817 || levelData[gridY][gridX] ==  883 || levelData[gridY][gridX] == 884){
//        
//        float x_dist = fabs(val - ((gridX*TILE_SIZE)));
//        
//        player->position.x += x_dist + 0.001;
//        player->velocity.x = 0;
//        player->collidedLeft = true;
//        
//    }
//    else {
//        player->collidedLeft = false;
//    }
//    if (levelData[gridY][gridX] == 1186 || levelData[gridY][gridX] == 1187 ){
//        state = GAME_OVER;
//    }
}
void Platformer::checkRightCollision(Entity* entity) {
    
    int gridX, gridY;
//    float val = (player->position.x - (player->sprite->width / 2));
    float val = entity->position.x + (entity->sprite->width * entity->scale.x);

    worldToTileCoordinates(val, entity->position.y, &gridX, &gridY);
    
    entity->collidedRight = false;
    
    for (int i : collideTiles) {
        if (levelData[gridY][gridX] == i){
            
            float x_dist = fabs(val - ((gridX*TILE_SIZE)));
            entity->position.x -= x_dist + 0.001;
            entity->velocity.x = 0;
            entity->collidedRight = true;
        }
   
    }
    if (levelData[gridY][gridX] == 65) {
        if (state == LEVEL_ONE) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelTwo();
            state = LEVEL_TWO;
        }
        else if (state == LEVEL_TWO) {
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelThree();
            state = LEVEL_THREE;
        }
        else if (state == LEVEL_THREE){
            player->position.x = 2.7;
            player->position.y = -6;
            flies.clear();
            powerups.clear();
            buttons.clear();
            blocks.clear();
            keys.clear();
            readLevelFour();
            state = LEVEL_FOUR;
        }
        else {
            state = GAME_OVER;
        }
    }
}
bool Platformer::checkCollision(Entity* a, Entity* b) {

    if (a == b) {
        return false;
    }
    a->buildMatrix();
    b->buildMatrix();
    
    Matrix aInverse = a->matrix.inverse();
    Matrix bInverse = b->matrix.inverse();
    
    //    float aWidth = a->sprite->width * a->scale_x * 0.9f;
    float aWidth = a->sprite->width * a->scale.x;
    float aHeight = a->sprite->height * a->scale.y;
    float bWidth = b->sprite->width * b->scale.x;
    float bHeight = b->sprite->height * b->scale.y;
    
    Vector aTL = Vector(-aWidth, aHeight);
    Vector aBL = Vector(-aWidth, -aHeight);
    Vector aBR = Vector(aWidth, -aHeight);
    Vector aTR = Vector(aWidth, aHeight);
    
    aTL = a->matrix * aTL;
    aBL = a->matrix * aBL;
    aBR = a->matrix * aBR;
    aTR = a->matrix * aTR;
    
    aTL = bInverse * aTL;
    aBL = bInverse * aBL;
    aBR = bInverse * aBR;
    aTR = bInverse * aTR;
    
    float minX, maxX, minY, maxY;
    minX = min(min(min(aTL.x, aBL.x), aTR.x), aBR.x);
    maxX = max(max(max(aTL.x, aBL.x), aTR.x), aBR.x);
    if (!(minX <= bWidth && maxX >= -bWidth)) {
        return false;
    }
    minY = min(min(min(aTL.y, aBL.y), aTR.y), aBR.y);
    maxY = max(max(max(aTL.y, aBL.y), aTR.y), aBR.y);
    if (!(minY <= bHeight && maxY >= -bHeight)) {
        return false;
    }
    
    Vector bTL = Vector(-bWidth, bHeight);
    Vector bBL = Vector(-bWidth, -bHeight);
    Vector bBR = Vector(bWidth, -bHeight);
    Vector bTR = Vector(bWidth, bHeight);
    
    bTL = b->matrix * bTL;
    bBL = b->matrix * bBL;
    bTR = b->matrix * bTR;
    bBR = b->matrix * bBR;
    
    bTL = aInverse * bTL;
    bBL = aInverse * bBL;
    bBR = aInverse * bBR;
    bTR = aInverse * bTR;
    
    minX = min(min(min(bTL.x, bBL.x), bTR.x), bBR.x);
    maxX = max(max(max(bTL.x, bBL.x), bTR.x), bBR.x);
    if (!(minX <= aWidth && maxX >= -aWidth)) {
        return false;
    }
    minY = min(min(min(bTL.y, bBL.y), bTR.y), bBR.y);
    maxY = max(max(max(bTL.y, bBL.y), bTR.y), bBR.y);
    if (!(minY <= aHeight && maxY >= -aHeight)) {
        return false;
    }
    return true;
 
}
void Platformer::checkYCollisions(Entity* entity) {
        checkBotCollision(entity);
    
        checkTopCollision(entity);
    
}
void Platformer::checkXCollisions(Entity* entity) {
     checkLeftCollision(entity);
     checkRightCollision(entity);
}
void Platformer::shoot(Entity* shooter, vector<Entity*> bullets, int& index) {
//    if (bullets[index]) {
    
        cout << "shoot";
        bullets[index]->visible = true;
        bullets[index]->velocity.x = fabs(bullets[index]->velocity.x);
        
        if (shooter->flipped) {
            bullets[index]->velocity.x *= -1;
            bullets[index]->position.x = shooter->position.x - shooter->sprite->width/2;
            bullets[index]->position.y = shooter->position.y + shooter->sprite->height * shooter->scale.y;
//            bullets[index]->position.x = shooter->position.x;
//            bullets[index]->position.y = shooter->position.y;
        }
        else {
//            
            bullets[index]->position.x = shooter->position.x + shooter->sprite->width/2;
            bullets[index]->position.y = shooter->position.y + shooter->sprite->height * shooter->scale.y;
            
//            bullets[index]->position.x = shooter->position.x;
//            bullets[index]->position.y = shooter->position.y;
        }
        index++;
        if (index > MAX_BULLETS - 1) {
            bIndex = 0;
        }
 //   }
}

void Platformer::fixedUpdate() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) done = true;
        else if(event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                //                if (player->collidedBot) {
                player->velocity.y = 7;
                
//                player->scale.y = mapValue(fabs(player->velocity.y), 0.0, 2.0, 0.4, 0.8);
//                player->scale.x = mapValue(fabs(player->velocity.y), 2.0, 0.0, 0.6, 0.4);
                //                Mix_PlayChannel(-1, jump, 0);
                //                }
            }
            else if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                if (player->bullets.size() > 0) shoot(player, player->bullets, bIndex);
            }
            else {
                
//                player->scale.y = 0.2;
//                player->scale.x = 0.95;
                player->velocity.y = 0;
            }
        }
    }
    
//    player->velocity.x = lerp(player->velocity.x, 0.0f, FIXED_TIMESTEP*friction);
//    player->velocity.x += player->acceleration.x * FIXED_TIMESTEP;
    player->position.x += player->velocity.x * FIXED_TIMESTEP;
    
//    player->velocity_x = lerp(player->velocity_x, 0.0f, FIXED_TIMESTEP*friction);
//    player->velocity_x += player->acceleration_x * FIXED_TIMESTEP;
//    player->x += player->velocity_x * FIXED_TIMESTEP;
    
    
    checkXCollisions(player);
    
    player->velocity.y = lerp(player->velocity.y, 0.0f, FIXED_TIMESTEP*gravity);
    player->velocity.y += player->acceleration.y * FIXED_TIMESTEP;
    player->position.y += player->velocity.y * FIXED_TIMESTEP;
    player->velocity.y -= gravity*1.2 * FIXED_TIMESTEP;
    

    for (Entity* bullet : player->bullets) {
        if (bullet->visible){
            bullet->position.x += bullet->velocity.x;
        
            bullet->lifeTime += FIXED_TIMESTEP;
            
            if (bullet->lifeTime > 7) {
                bullet->visible = false;
            }
        }
        
    }
    for (Entity* fly : flies) {
        for (Entity* bullet : player->bullets)
        
            if (checkCollision(fly, bullet)){
                fly->sprite->u = 143.0/353.0;
                fly->sprite->v = 0;
                fly->sprite->width = 59.0/353.0;
                fly->sprite->height = 33.0/153.0;
                fly->dead = true;
               // fly->visible = false;
            }
        
        if (fly->angry && !fly->dead) {
            
            Vector direction;
            direction.x = player->position.x - fly->position.x;
            direction.y = player->position.y - fly->position.y;
            
            
            direction.normalize();
            
            fly->position.y += direction.y*0.02;
            fly->position.x += direction.x*0.02;
            
        }
        else  if (!fly->dead) {
          
        
        fly->position.x += noise1(perlinValue)/15;
        fly->position.y += noise1(perlinValue+ 1.0f)/15;
            
        }
        else{
            
            fly->position.y -= gravity * FIXED_TIMESTEP;
            checkXCollisions(fly);
            checkYCollisions(fly);
            
        }
        
        if (fly->collidedBot || fly->collidedTop || fly->collidedRight || fly->collidedLeft){
            fly->angry = false;
        }
        
        
        if (checkCollision(player, fly) && !fly->dead) {
            fly->angry = true;
            for (auto it = player->hearts.rbegin(); it != player->hearts.rend(); ++it) {
                
                //                for (Entity* heart : player->hearts) {
                
                
                if ((*it)->color == "Empty") continue;
                
                
                else if ((*it)->color == "Full"){
                    (*it)->sprite->v = 0.0;
                    (*it)->color = "Half";
                    
                    break;
                    
                    
                }
                else if ((*it)->color == "Half") {
                    (*it)->sprite->v = 47.0/269.0;
                    (*it)->color = "Empty";
                    
                    break;
                    
                }
                
                
            }
            if (player->flipped){
                player->position.x += 1;
                //    player->position.y += 0.3;
                //    player->velocity.y = 0;
            }
            else{
                player->position.x -= 1;
                //    player->position.y += 0.3;
                //    player->velocity.y = 0;
            }
            
           
        }
        

    }

    
      checkYCollisions(player);

    
    for (Entity* b : blocks) {
        if (b->visible){
        if (checkCollision(player, b)) {
//            float b_left = b->position.x - b->sprite->width / 2;
//            float p_right = player->position.x + (player->sprite->width * player->scale.x);
//            
            float x_dist = fabs(player->position.x - b->position.x);
            float x_maxDist = player->sprite->width + b->sprite->width + 0.1;
            float x_pen = fabs(x_dist - x_maxDist);
            
     
                player->position.x -= x_pen + 0.0001;
                player->velocity.x = 0;
            
            
        }
        }
    }
    for (Entity* but : buttons) {
        if (checkCollision(player, but)) {
            but->sprite->u = 360.0/576.0;
            but->sprite->v = 288.0/576.0;
            
            for (Entity* b : blocks) {
                b->visible = false;
            }
        }
    }
    for (Entity* c : coins) {
        if (checkCollision(player, c)) {
            if (c->visible) {
                player->playerCoins += 1;
                c->visible = false;
            }
        }
    }
    for (Entity* s : stars) {
        if (checkCollision(player, s)) {
            if (s->visible) {
                player->playerCoins += 10;
                s->visible = false;
            }
        }
    }
    for (Entity* k : keys) {
        if (checkCollision(player, k)) {
            if (k->visible) {
                player->playerKeys.push_back(k);
                k->visible = false;
                for (Entity* k_hud : keyHuds) {
                   // for (Entity* key : keys) {
                        if (k->color == k_hud->color) {
                            if (k_hud->color == "Blue" ) {
                                k_hud->sprite->u = 146.0/269.0;
                                k_hud->sprite->v = 147.0/269.0;
                            }
                            else if (k_hud->color == "Green") {
                                k_hud->sprite->u = 192.0/269.0;
                                k_hud->sprite->v = 122.0/269.0;
                            }
                            else if (k_hud->color == "Orange") {
                                k_hud->sprite->u = 193.0/269.0;
                                k_hud->sprite->v = 80.0/269.0;
                            }
                            else if (k_hud->color == "Yellow") {
                                k_hud->sprite->u = 146.0/269.0;
                                k_hud->sprite->v = 189.0/269.0;
                            }
                        }
                    //}
                }
            }
        }
    }
    for (Entity* p : powerups) {
        if (checkCollision(player, p)) {
            
            for (Entity* k : player->playerKeys) {
                if (k->color == p->color) {
                    p->visible = false;
                    
                    for (Entity* k_hud : keyHuds) {
                        if (k_hud->color == p->color) {
                            if (k_hud->color == "Blue" ) {
                                k_hud->sprite->u = 150.0/269.0;
                                k_hud->sprite->v = 38.0/269.0;
                            }
                            else if (k_hud->color == "Green") {
                                k_hud->sprite->u = 104.0/269.0;
                                k_hud->sprite->v = 38.0/269.0;
                            }
                            else if (k_hud->color == "Orange") {
                                k_hud->sprite->u = 192.0/269.0;
                                k_hud->sprite->v = 164.0/269.0;
                            }
                            else if (k_hud->color == "Yellow") {
                                k_hud->sprite->u = 147.0/269.0;
                                k_hud->sprite->v = 80.0/269.0;
                            }
                            
                        }
                    }
                    
                    if (p->color == "Blue" ) {
                        player->speed = 3.5;

                      
                    }
                    else if (p->color == "Green") {
                      
                    }
                    else if (p->color == "Orange") {
                        if (player->bullets.size() == 0){
                        for (int i = 0; i < MAX_BULLETS; i++) {
                            Entity* bullet = new Entity(items, 0.0/576.0, 435.0/576.0, 70.0/576.0, 70.0/576.0, 0, 0, 0.6, 0.6, 0);
                            bullet->visible = false;
                            bullet->velocity.x = 0.1;
                            bullet->lifeTime = 0;
                            player->bullets.push_back(bullet);
                        }
                        }
                        
                    }
                    else if (p->color == "Yellow") {
                      
                    }
                }
            }
        }
    }

 
    bool dead = true;
    for (Entity* heart : player->hearts) {
        
        if (heart->color != "Empty") dead = false;
    }
    if (dead) {
        player->visible = false;
        state = GAME_OVER;
        
    }

  
}
void Platformer::Update(float elapsed){
    switch(state) {
        case MAIN_MENU:
            UpdateMainMenu(elapsed);
            break;
        case LEVEL_ONE:
            UpdateGameLevel(elapsed);
            break;
        case LEVEL_TWO:
            UpdateGameLevel(elapsed);
            break;
        case LEVEL_THREE:
            UpdateGameLevel(elapsed);
            break;
        case LEVEL_FOUR:
            UpdateGameLevel(elapsed);
            break;
        case GAME_OVER:
            UpdateGameOver();
            break;
    }
}
void Platformer::UpdateMainMenu(float elapsed) {
    glMatrixMode(GL_MODELVIEW);
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) done = true;
        else if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                state = LEVEL_ONE;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                done = true;
            }
        }
    }
    
}
void Platformer::UpdateGameOver() {
    glMatrixMode(GL_MODELVIEW);
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) done = true;
        else if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                resetGame();
                state = LEVEL_ONE;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                done = true;
            }
        }
    }
}
void Platformer::UpdateGameLevel(float elapsed) {
    glMatrixMode(GL_MODELVIEW);
    
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) done = true;
        else if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.scancode == SDL_SCANCODE_Q) {
                done = true;
            }
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_RIGHT]){
        
      
        player->flipped = false;
        player->velocity.x = player->speed;
        
        if (player->collidedBot){
            
        
        animationElapsed += elapsed;
        if (animationElapsed > 1.0/framesPerSecond) {
            anim_index++;
            animationElapsed = 0.0;
            
            if (anim_index > numFrames - 1) {
                anim_index = 0;
            }
        }
        }
    }
    else if(keys[SDL_SCANCODE_LEFT]){
        
       
        player->flipped = true;
          player->velocity.x = -player->speed;
        
        if (player->collidedBot) {
            
      
           
        animationElapsed += elapsed;
        if (animationElapsed > 1.0/framesPerSecond) {
            anim_index++;
            animationElapsed = 0.0;
            
            if (anim_index > numFrames - 1) {
                anim_index = 0;
            }
        }
        }
        
    }
    
    else {
        anim_index = 10;
        player->velocity.x = 0;
    }
    
    animationTime += elapsed;
    animationValue = mapValue(animationTime, 0.0, 5.0, 0.0, 3.0);
    
   perlinValue += elapsed;
    
    for (Entity* fly : flies) {
        if (!fly->dead){
        
        fly_animationElapsed += elapsed;
        if (fly_animationElapsed > 0.3) {
            fly->sprite->u = 0;
            fly->sprite->v = 0;
            fly->sprite->width = 75.0/353.0;
            fly->sprite->height = 31.0/153.0;
            
        }
        else {
            fly->sprite->u = 0;
            fly->sprite->v = 32.0/153.0;
            fly->sprite->width = 72.0/353.0;
            fly->sprite->height = 36.0/153.0;
        }
        if (fly_animationElapsed > 0.6) fly_animationElapsed = 0;
        }
    }
    
}
void Platformer::Render() {
    switch(state) {
        case MAIN_MENU:
            RenderMainMenu();
            break;
        case LEVEL_ONE:
            RenderGameLevel();
            break;
        case LEVEL_TWO:
            RenderGameLevel();
            break;
        case LEVEL_THREE:
            RenderGameLevel();
            break;
        case LEVEL_FOUR:
            RenderGameLevel();
            break;
        case GAME_OVER:
            RenderGameOver();
            break;
    }
}
void Platformer::RenderMainMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    
//       DrawText(font_tex, "Press Up to jump/fly.", lerp(0.0, 1.0, animationValue), 0.0, 0.05, 0.001, 0, 0, 0, 0);
    DrawText(font_tex, "Press Up to jump/fly.", -1, 1, 0.08, 0.001, 0, 0, 0, 0);
    DrawText(font_tex, "Collect coins. Avoid enemies.", -1, 0.6, 0.08, 0.001, 0, 0, 0, 0);
    DrawText(font_tex, "Reach the end to win.", -1, 0.3, 0.08, 0.001, 0, 0, 0, 0);
    DrawText(font_tex, "Press Enter to begin.", -1, -0.1, 0.08, 0.001, 0, 0, 0, 0);
    DrawText(font_tex, "Press Q to quit.", -1, -0.3, 0.08, 0.001, 0, 0, 0, 0);
    
//
//    
//    DrawText(font_tex, "GAME", 0, 0, 0.25, 0.01, 0, 0, 0, 0);
//    DrawText(font_tex, "OVER", -0.9, 0.34, 0.25, 0.01, 0, 0, 0, 0);
//    DrawText(font_tex, "Press Enter to play again.", -0.32, -.4, 0.05, 0.0005, 0, 0, 0, 0);
    
    SDL_GL_SwapWindow(window);
}
void Platformer::RenderGameLevel() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    float b= player->position.x * 600.0f;
    float c = ((int)b) / 600.0f;
    
    float d = player->position.y * 600.0f;
    float e = ((int)d) / 600.0f;
    player->position.x = c;
    player->position.y = e;
 
    Matrix mat;
    mat.identity();
    mat.m[3][0] = -player->position.x;
    mat.m[3][1] = -player->position.y;
    
    
    glMultMatrixf(mat.ml);

    
    BuildLevel(sprite_sheet);
    

    for (Entity* b : blocks) {
        b->Draw();
    }
    for (Entity* cloud : clouds) {
        cloud->Draw();
    }
    for (Entity* c : coins) {
        c->Draw();
    }
    for (Entity* s : stars) {

        s->Draw();
    }
    for (Entity* k : keys) {
        k->Draw();
    }
    for (Entity* p : powerups) {

        p->Draw();
    }
    for (Entity* but : buttons) {
        but->Draw();
    }
    for (Entity* e : slimes) {
        e->Draw();
    }
    for (Entity* e : flies) {
        
        e->Draw();
    }
    for (Entity* b : player->bullets) {
        if (b->visible){
            b->Draw();
        }
    }
    float y_offset = 1.6;
    float x_offset = 2.1;
    for (Entity* heart : player->hearts) {
        heart->position.x = player->position.x + x_offset;
        heart->position.y = player->position.y + y_offset;
        x_offset += 0.15;
        
        heart->Draw();
    }
    

    coinHud->position.x = player->position.x + 2.4;
    coinHud->position.y = player->position.y + 1.48;
    coinHud->Draw();
    DrawText(font_tex, ":", coinHud->position.x + 0.07, coinHud->position.y - 0.005, 0.07, 0.0005, 0, 0, 0, 0);
    string coins = to_string(player->playerCoins);
    DrawText(font_tex, coins, coinHud->position.x + 0.16, coinHud->position.y - 0.005, 0.07, 0.0005, 0, 0, 0, 0);
    
  
    float key_offset_x = 0.165;
    for (Entity* k_hud : keyHuds) {
        if (k_hud->visible) {
        k_hud->position.x = coinHud->position.x + key_offset_x;
        k_hud->position.y = coinHud->position.y - 0.12;
            k_hud->scale.x = 0.4;
            k_hud->scale.y = 0.4;
            k_hud->Draw();
            key_offset_x -= 0.2;
        }

    }
 
    player->DrawUniform(anim_index);
    
    SDL_GL_SwapWindow(window);
}

void Platformer::RenderGameOver() {
    glClear(GL_COLOR_BUFFER_BIT);
    

    if (player->visible) {
        DrawText(font_tex, "YOU", -1, 0.6, 0.25, 0.01, 0, 0, 0, 0);
        DrawText(font_tex, "WIN! :)", -0.9, 0.34, 0.25, 0.01, 0, 0, 0, 0);
        DrawText(font_tex, "Press Enter to play again.", -0.32, -.4, 0.05, 0.0005, 0, 0, 0, 0);
    }
    else {
//        DrawText(font_tex, "GAME", -1, 0.6, 0.25, 0.01, 0, 0, 0, 0);
        DrawText(font_tex, "OVER", 0, 3, 0.25, 0.01, 0, 0, 0, 0);
        DrawText(font_tex, "GAME", -0.9, 0.34, 0.25, 0.01, 0, 0, 0, 0);
        DrawText(font_tex, "Press Enter to play again.", -0.32, -.4, 0.05, 0.0005, 0, 0, 0, 0);
    }
    SDL_GL_SwapWindow(window);
}
bool Platformer::UpdateAndRender() {
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    float fixedElapsed = elapsed + timeLeftOver;
    if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        if (state == LEVEL_ONE || state == LEVEL_TWO || state == LEVEL_THREE || state == LEVEL_FOUR) fixedUpdate();
   
    }
    timeLeftOver = fixedElapsed;
    
    Update(elapsed);
    Render();
    return done;
}
