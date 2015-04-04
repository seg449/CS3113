#include "Sprite.h"

Sprite::Sprite(float x, float y, float vX, float vY, float aX, float aY, float fX, float fY, bool Static) {
	this->x = x;
	this->y = y;
	velocity_x = vX;
	velocity_y = vY;
	acceleration_x = aX;
	acceleration_y = aY;
	friction_x = fX;
	friction_y = fY;
	isStatic = Static;
	enableCollisions = true;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	isVisible = true;
}

void Sprite::Render(float transX, float transY){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(transX, transY, 0.0);
	//cout << x << "\t" << y << "\t" << width << "\t" << height << endl;
	glTranslatef(x, y, 0.0f);
	GLfloat quad[] = {-width * 0.2f, height * 0.2f, -width * 0.2f, -height * 0.2f, width * 0.2f, -height * 0.2f, width * 0.2f, height * 0.2f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quads[] = {u, v, u, v + height, u + width, v + height, u + width, v};
	glTexCoordPointer(2, GL_FLOAT, 0, quads);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
bool Sprite::collidesWith(Sprite *sprite){
	if (x + width / 2 < sprite->x - sprite->width / 2) return false;
	if (x - width / 2 > sprite->x + sprite->width / 2) return false;
	if (y + height / 2 < sprite->y -sprite->height / 2) return false;
	if (y - height / 2 > sprite->y + sprite->height / 2) return false;
	return true;
}


