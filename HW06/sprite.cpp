#include "sprite.h"
#include <iostream>
using namespace std;

sprite::sprite(GLuint ID, float u, float v, float w, float h, float x, float y, float velocityX, float velocityY, float constX, float constY, float s, float rotation, bool collidedTop, bool collidedBottom, bool collidedLeft, bool collidedRight){
	this->x = x;
	this->y = y;
	this->u = u;
	this->v = v;
	this->s = s;
	lives = 0;
	width = w;
	height = h;
	this->constX = constX;
	this->constY = constY;
	vX = velocityX;
	vY = velocityY;
	this->ID = ID;
	r = rotation;
	collidedLeft = false;
	collidedRight = false;
	collidedBottom = false;
	collidedTop = false;	
}

void sprite::Render(){
	cout << width << "\t" << height << endl;

	matrixTransformations();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(m.ml);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID);
	GLfloat quad[] = { -width * constX, height * constY, -width * constX, -height * constY, width * constX, -height * constY, width * constX, height * constY };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

void sprite::matrixTransformations(){
	//this function will generate the scale matrix times the rotation matrix times the translation matrix
	//first reset the current matrix to the identity matrix
	/*m.identity();
	Matrix translation, scale, rotation;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			if (i == 0){
				if (j == 0){
					scale.m[i][j] = constX;
					rotation.m[i][j] = cos(r);
				}
				else if (j == 1){
					rotation.m[i][j] = sin(r);
				}
			}
			else if (i == 1){
				if (j == 0) rotation.m[i][j] = -sin(r);
				else if (j == 1){
					scale.m[i][j] = constY;
					rotation.m[i][j] = cos(r);
				}
			}
			else if (i == 3){
				if (j == 0) translation.m[i][j] == x;
				else if (j == 1) translation.m[i][j] == y;
			}
		}
	}*/
	Matrix scale;
	scale.m[0][0] = constX;
	scale.m[1][1] = constY;

	Matrix rotate;
	rotate.m[0][0] = cos(r);
	rotate.m[0][1] = sin(r);
	rotate.m[1][0] = -sin(r);
	rotate.m[1][1] = cos(r);

	Matrix translate;
	translate.m[3][0] = x;
	translate.m[3][1] = y;

	m = scale * rotate * translate;
}