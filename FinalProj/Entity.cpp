//
//  Entity.cpp
//  Platformer
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard on 5/6/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//

#include "Entity.h"

float lerp(float v0, float v1, float t){
    return (1.0f - t) * v0 + t * v1;
}

SheetSprite::SheetSprite() : textureID(0), u(0), v(0), width(1.0), height(1.0) {}

SheetSprite::SheetSprite(unsigned int texID, float u, float v, float w, float h) : textureID(texID), u(u), v(v), width(w), height(h) {}
SheetSprite::SheetSprite(unsigned int texID, int countX, int countY) : textureID(texID), countX(countX), countY(countY) {
    
    width = 1.0/(float)countX;
    height = 1.0/(float)countY;
}

void Entity::buildMatrix() {
    
    Matrix translate;
    Matrix scaling;
    Matrix rotate;
    Matrix final;
    
    translate.identity();
    translate.m[3][0] = position.x;
    translate.m[3][1] = position.y;
    
    scaling.identity();
    
    if (flipped) {
        scaling.m[0][0] = -scale.x;
        scaling.m[1][1] = scale.y;
    }
    else {
        scaling.m[0][0] = scale.x;
        scaling.m[1][1] = scale.y;
    }
    
    rotate.identity();
    rotate.m[0][0] = cosf(rotation);
    rotate.m[1][0] = sinf(rotation);
    rotate.m[0][1] = -sinf(rotation);
    rotate.m[1][1] = cosf(rotation);
    
    final.identity();
    final = translate * scaling * rotate;

    matrix = final;
}

Entity::Entity(SheetSprite* sprite, float x, float y, float sx, float sy, float r) : sprite(sprite), position(x,y), scale(sx,sy), rotation(r) {}

Entity:: Entity(unsigned int tex, float u, float v, float w, float h, float x, float y, float sx, float sy, float r) : position(x,y), scale(sx,sy),rotation(r) {
    
    sprite = new SheetSprite(tex, u, v, w, h);
}

Entity::~Entity() {
    if (sprite) {
        delete sprite;
    }
    sprite = nullptr;
    for (Entity* heart : hearts) {
        delete heart;
    }
    playerKeys.clear();
    
}
void Entity::Draw() {
  
    if (visible) {

        buildMatrix();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sprite->textureID);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf(matrix.ml);
        
        GLfloat quad[] = {-sprite->width, sprite->height, -sprite->width, (-sprite->height),
            sprite->width, -sprite->height, sprite->width, sprite->height};
        
        GLfloat quadUVs[] = {sprite->u, sprite->v, sprite->u, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v};
        
        
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        
        
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}
void Entity::DrawUniform(int index) {

    if (visible) {
      
        buildMatrix();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sprite->textureID);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        
        glMultMatrixf(matrix.ml);
        
        double offset = 0.084;
        
        float u = ((float)(((int)index) % sprite->countX) / (float) sprite->countX) - offset;
        float v = ((float)(((int)index) / sprite->countX) / (float) sprite->countY);
        
        GLfloat quad[] = {-(sprite->width), sprite->height, -(sprite->width), -(sprite->height),
            sprite->width, -(sprite->height), sprite->width, sprite->height};
        
        GLfloat quadUVs[] = { u, v,
            u, v+sprite->height,
            u+sprite->width, v+sprite->height,
            u+sprite->width, v
        };
        
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

