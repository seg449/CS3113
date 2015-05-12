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

Color::Color() {}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

ParticleEmitter::ParticleEmitter() {}
ParticleEmitter::~ParticleEmitter() {
    particles.clear();
}
ParticleEmitter::ParticleEmitter(unsigned int particleCount){
    
    position.x = 3;
    position.y = -6;
    
    gravity.x = 0;
    gravity.y = 4;
    
    velocity.x = 0;
    velocity.y = 0;
    
    startColor.r = 1.0;
    startColor.g = 0.0;
    startColor.b = 0.0;
    startColor.a = 0.0;
  
    endColor.r = 0.15;
    endColor.g = 0.55;
    endColor.b = 0.65;
    endColor.a = 0.0;
    
    maxLifetime = 5.0;
    
    for (int i = 0; i < particleCount; i++) {
        Particle p;
        p.lifetime = rand()%5;
        p.position.x = position.x;
        p.position.y = position.y;
        p.velocity.x = cosf(2*M_PI * rand() / (RAND_MAX + 1));
        p.velocity.y = sinf(2*M_PI * rand() / (RAND_MAX + 1));
        particles.push_back(p);
    }
}

void ParticleEmitter::Update(float elapsed) {
    
    for (Particle p : particles) {
        
        p.position.x += p.velocity.x * elapsed;
        p.position.y += p.velocity.y * elapsed;
        p.position.y -= gravity.y * elapsed;
        
        p.lifetime += elapsed;
        if (p.lifetime > maxLifetime) {
            p.lifetime = 0;
            p.position.x = position.x;
            p.position.y = position.y;
        }
        
    }
    
}
void ParticleEmitter::Render() {
    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, sprite->textureID);
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glMultMatrixf(matrix.ml);
//    
//    GLfloat quad[] = {-sprite->width, sprite->height, -sprite->width, (-sprite->height),
//        sprite->width, -sprite->height, sprite->width, sprite->height};
//    
//    GLfloat quadUVs[] = {sprite->u, sprite->v, sprite->u, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v};
//    
//    
//    glVertexPointer(2, GL_FLOAT, 0, quad);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    
//    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDrawArrays(GL_QUADS, 0, 4);
//    
//    
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
    
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //    glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
    vector<float> particleVertices;
    for(int i=0; i < particles.size(); i++) {
        particleVertices.push_back(particles[i].position.x);
        particleVertices.push_back(particles[i].position.y);
    }
  
    
    vector<float> particleColors;
    for(int i=0; i < particles.size(); i++) {
        float m = (particles[i].lifetime/maxLifetime);
        particleColors.push_back(lerp(startColor.r, endColor.r, m));
        particleColors.push_back(lerp(startColor.g, endColor.g, m));
        particleColors.push_back(lerp(startColor.b, endColor.b, m));
        particleColors.push_back(lerp(startColor.a, endColor.a, m));
    }
    
    glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, particleColors.data());
    glEnableClientState(GL_COLOR_ARRAY);
    
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(3.0f);
    glDrawArrays(GL_POINTS, 0, particleVertices.size()/2);
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    //    glDrawArrays(GL_QUADS, 0, 4);
        glPopMatrix();

    
}


//void SheetSprite::Draw(GLfloat scale, float x, float y, bool flipped) {
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    
//    GLfloat quad[] = {-width * scale, height * scale, -width * scale, (-height * scale),
//        width * scale, -height * scale, width * scale, height * scale };
//    
//    GLfloat quadUVs[] = {u, v, u, v+height, u+width, v+height, u+width, v};
//    
//    glMatrixMode(GL_MODELVIEW);
//    
//    glTranslatef(x, y, 0.0);
//    
//    if (flipped) {
//        glScalef(-1, 1, 1);
//    }
//    else {
//        glScalef(1, 1, 1);
//    }
//    glVertexPointer(2, GL_FLOAT, 0, quad);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    
//    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDrawArrays(GL_QUADS, 0, 4);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
//}
//void SheetSprite::DrawUniform(int index, float x, float y, float scale) {
//    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    //    glEnable(GL_TEXTURE_2D);
//    //    glBindTexture(GL_TEXTURE_2D, textureID);
//    //
//    //    countX = 3;
//    //    countY = 4;
//    //
//    //    float u = (float)(((int)index) % countX) / (float) countX;
//    //    float v = (float)(((int)index) / countX) / (float) countY;
//    //    float spriteWidth = 1.0/(float)countX;
//    //    float spriteHeight = 1.0/(float)countY;
//    //
//    //    GLfloat quad[] = {-spriteWidth, spriteHeight, -spriteWidth, (-spriteHeight),
//    //        spriteWidth, -spriteHeight, spriteWidth, spriteHeight};
//    //
//    //    GLfloat quadUVs[] = { u, v,
//    //        u, v+spriteHeight,
//    //        u+spriteWidth, v+spriteHeight,
//    //        u+spriteWidth, v
//    //    };
//    //    glMatrixMode(GL_MODELVIEW);
//    //
//    //    glLoadIdentity();
//    //    glTranslatef(x, y, 0.0);
//    //    glScalef(scale, scale, 0);
//    //
//    //    glVertexPointer(2, GL_FLOAT, 0, quad);
//    //    glEnableClientState(GL_VERTEX_ARRAY);
//    //
//    //    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
//    //    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    //
//    //    glEnable(GL_BLEND);
//    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    //    glDrawArrays(GL_QUADS, 0, 4);
//    //    glDisable(GL_TEXTURE_2D);
//
//}

void Entity::buildMatrix() {
    
    Matrix translate;
    Matrix scaling;
    Matrix rotate;
    Matrix final;
//    Matrix flip;
    
//    flip.identity();
//    flip.m[0][0] = -1;
    
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
//    if (!flipped)
    final = translate * scaling * rotate;
//    else {
//        final = flip * scaling * translate * rotate;
//    }
//    
    matrix = final;
}

Entity::Entity(SheetSprite* sprite, float x, float y, float sx, float sy, float r) : sprite(sprite), position(x,y), scale(sx,sy), rotation(r) {}

Entity:: Entity(unsigned int tex, float u, float v, float w, float h, float x, float y, float sx, float sy, float r) : position(x,y), scale(sx,sy),rotation(r) {
    
    sprite = new SheetSprite(tex, u, v, w, h);
}

//Entity::Entity(SheetSprite* sprite, float x, float y, float sx, float sy, float r) : sprite(sprite), x(x), y(y), scale_x(sx), scale_y(sy), rotation(r) {}
//
//Entity:: Entity(unsigned int tex, float u, float v, float w, float h, float x, float y, float sx, float sy, float r) : x(x), y(y), scale_x(sx), scale_y(sy), rotation(r) {
//    
//    sprite = new SheetSprite(tex, u, v, w, h);
//}


Entity::~Entity() {
    if (sprite) {
        delete sprite;
    }
    sprite = nullptr;
    for (Entity* heart : hearts) {
        delete heart;
    }
    for (Entity* key : playerKeys) {
        //delete key;
    }
    
}
void Entity::Build() {
    
    
}
void Entity::Draw() {
    //    if (visible) {
    //        sprite->Draw(scale,x,y,flip);
    //    }
    
    if (visible) {
        
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, sprite->textureID);
//        glMatrixMode(GL_MODELVIEW);
//        glPushMatrix();
//        
//        GLfloat quad[] = {-sprite->width * scale.x, sprite->height * scale.y, -sprite->width * scale.x, (-sprite->height * scale.y),
//            sprite->width * scale.x, -sprite->height * scale.y, sprite->width * scale.x, sprite->height * scale.y };
//        
//        GLfloat quadUVs[] = {sprite->u, sprite->v, sprite->u, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v};
//        
//        glMatrixMode(GL_MODELVIEW);
//        
//        glTranslatef(position.x, position.y, 0.0);
//        
//        if (flipped) {
//            glScalef(-1, 1, 1);
//        }
//        else {
//            glScalef(1, 1, 1);
//        }
//        glVertexPointer(2, GL_FLOAT, 0, quad);
//        glEnableClientState(GL_VERTEX_ARRAY);
//        
//        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
//        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//        
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glDrawArrays(GL_QUADS, 0, 4);
//        glPopMatrix();
//        glDisable(GL_TEXTURE_2D);

        
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
//    if (visible){
//        sprite->DrawUniform(index, x, y, scale);
//    }
    
    if (visible) {
        
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, sprite->textureID);
        
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, sprite->textureID);
//        glMatrixMode(GL_MODELVIEW);
//        glPushMatrix();
//        
//        float offset = 0.083;
//        float u = (float)(((int)index) % sprite->countX) / (float) sprite->countX - offset;
//        float v = (float)(((int)index) / sprite->countX) / (float) sprite->countY;
//       
//        GLfloat quad[] = {-(sprite->width), sprite->height, -(sprite->width), -(sprite->height),
//                        sprite->width, -(sprite->height), sprite->width, sprite->height};
//            
//        GLfloat quadUVs[] = { u, v,
//                        u, v+sprite->height,
//                        u+sprite->width, v+sprite->height,
//                        u+sprite->width, v
//                    };
//        glMatrixMode(GL_MODELVIEW);
//        
//        glTranslatef(position.x, position.y, 0.0);
//        glScalef(scale.x, scale.y, 0);
//        if (flipped) glScalef(-1, 1, 1);
//        
//        else {
//            glScalef(1, 1, 1);
//        }
//        
//        glVertexPointer(2, GL_FLOAT, 0, quad);
//        glEnableClientState(GL_VERTEX_ARRAY);
//        
//        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
//        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//        
//        
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glDrawArrays(GL_QUADS, 0, 4);
//        glPopMatrix();
//        glDisable(GL_TEXTURE_2D);
    
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
        
//        glLoadIdentity();
//        glMultMatrixf(matrix.ml);
        
//        glTranslatef(position.x, position.y, 0.0);
//        glScalef(scale.x, scale.y, 0);
//        if (flipped) glScalef(-1, 1, 1);
//        
//        else {
//            glScalef(1, 1, 1);
//        }

        
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
//        if (flipped) glScalef(-1, 1, 1);
//        
//        else {
//            glScalef(1, 1, 1);
//        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
//        GLfloat quad[] = {-sprite->width, sprite->height, -sprite->width, (-sprite->height),
//            sprite->width, -sprite->height, sprite->width, sprite->height};
//        
//        GLfloat quadUVs[] = {sprite->u, sprite->v, sprite->u, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v+sprite->height, sprite->u+sprite->width, sprite->v};
//        
//        
//        glVertexPointer(2, GL_FLOAT, 0, quad);
//        glEnableClientState(GL_VERTEX_ARRAY);
//        
//        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
//        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glDrawArrays(GL_QUADS, 0, 4);
//        
//        
//        glPopMatrix();
//        glDisable(GL_TEXTURE_2D);
//    }
//  
}