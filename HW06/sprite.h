#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "matrix.h"
using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class sprite{
public:
	sprite(GLuint ID, float u, float v, float w, float h, float x = 0.0f, float y = 0.0f, float velocityX = 0.0f, float velocityY = 0.0f, float constX = 1.0f, float constY = 1.0f, float s = 1.0f, float rotation = 1.0f, bool collidedTop = false, bool collidedBottom = false, bool collidedLeft = false, bool collidedRight = false);
	bool collidedTop, collidedBottom, collidedLeft, collidedRight;
	float vX, vY, x, y, u, v, s, r, width, height, constX, constY;
	GLuint ID;
	Matrix m;
	Vector vec;
	int lives;
	void Render();
	void matrixTransformations();
};