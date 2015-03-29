#include "Sprite.h"

Sprite::Sprite(GLuint id, float x, float y, float u, float v, float w, float h, float vX, float vY, float aX, float aY, float fX, float fY, bool Static) : id(id), x(x), y(y), u(u), v(v) {
	width = w;
	height = h;
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

void Sprite::Render(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	GLfloat quad[] = {width * 0.2f * -1.0f, height * 0.2f, width * 0.2f * -1.0f, -height * 0.2f, width * 0.2f * 1.0f, -height * 0.2f, width * 0.2f * 1.0f, height * 0.2f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quads[] = {u, v, u, v + height, u + width, v + height, u + width, v};
	glTexCoordPointer(2, GL_FLOAT, 0, quads);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}
bool Sprite::collidesWith(Sprite *sprite){
	if (x + width < x - sprite->width) return false;
	if (x - width > x + sprite->width) return false;
	if (y + height < sprite->y -sprite->height) return false;
	if (y - height > sprite->y + sprite->height) return false;
	return true;
}


