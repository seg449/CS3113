#include "sprite.h"
#include <iostream>

Sprite::Sprite(GLuint Texture, float X, float Y, float U, float V, float W, float H, bool Visible) {
		x = X;
		y = Y;
		u = U;
		v = V;
		w = W;
		h = H;
		visible = Visible;
		notDead = true;
		direction = 1.0;
		texture = Texture; 
		speed = 0.01f;
}
void Sprite::setx(float xPos) {x = xPos;}
void Sprite::sety(float yPos) {y = yPos;}
void Sprite::setu(float uPos) {u = uPos;}
void Sprite::setv(float vPos) {v = vPos;}
void Sprite::setStatus(bool status) {notDead = status;}
void Sprite::setVisible(bool type) {visible = type;}
float Sprite::getDirection(){return direction;}
float Sprite::getx() {return x;}
float Sprite::gety() {return y;}
float Sprite::getSpeed() {return speed;}
bool Sprite::getVisible() {return visible;}
bool Sprite::getStatus() {return notDead;}
float Sprite::getWidth(){return w;}
float Sprite::getHeight(){return h;}
void Sprite::drawSprite(float offset){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	GLfloat quad[] = {w * offset * -1.0f, h * offset, w * offset * -1.0f, h * offset * -1.0f, w * offset, h * offset * -1.0f, w * offset, h * offset};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quads[] = {u, v, u, v + h, u + w, v + h, u + w, v};
	glTexCoordPointer(2, GL_FLOAT, 0, quads);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}
