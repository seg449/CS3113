 
/*
Sebastien Emmanuel Gerard
2/17/15
CS3113
HW02
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

using namespace std;

SDL_Window* display;

class Entity { //define an entity class for paddles and ball
public:
	Entity(float xAxis, float yAxis, float wid, float hgt, GLuint text) {
		x = xAxis;
		y = yAxis;
		width = wid;
		height = hgt;
		rotation = 0.5f;
		texture = text;
		float speed = 0.0;
		float dirX = 0.0;
		float dirY = 0.0;
	}
	//Mutators and Accessors
	float getdirY() {return dirY;}
	void setYDirection(float newDir) {dirY = newDir;}
	float getX() {return x;}
	void setX(float newX) {x = newX;}
	float getSpeed() {return speed;}
	float getHeight() {return height;}
	float getY() {return y;}
	float getdirX() {return dirX;}
	void setY(float newY) {y = newY;}
	void setSpeed(float newSpeed) {speed = newSpeed;}
	void setXDirection(float newDir) {dirX = newDir;}
	float getWidth() {return width;}

//Methods
	void drawSprite() { //COPIED FROM CLASS!
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glRotatef(rotation, 0.0, 0.0, 1.0);
		GLfloat quad[] = { width*-0.5f, height*0.5f, width*-0.5f, height*-0.5f, width*0.5f, height*-0.5f, width*0.5f, height*0.5f };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}

private:
	float x, y, rotation, scale, width, height, speed, dirX, dirY;
	GLuint texture;
};

GLuint LoadTexture(const char* img){ //COPIED FROM CLASS
	SDL_Surface* image = IMG_Load(img);
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SDL_FreeSurface(image);
	return id;
}

void Setup(){  //Set up and initialize 
	SDL_Init(SDL_INIT_VIDEO);
	display = SDL_CreateWindow("HW02", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(display);
	SDL_GL_MakeCurrent(display, glContext);
	//create the viewport
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}
bool ProcessEvents(SDL_Event &event, const Uint8 *key, Entity* pOne, Entity* pTwo, float timeElapsed){
	//Deal with keyboard input
	while (SDL_PollEvent(&event)) {  
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}
	if(key[SDL_SCANCODE_UP] == 1 && pOne->getY() < .85){ //playerOne
		pOne->setY(pOne->getY() + timeElapsed); //don't want to go below the screen
	}
	else if(key[SDL_SCANCODE_DOWN] == 1 && pOne->getY() > -.85){ //playerOne
		pOne->setY(pOne->getY() - timeElapsed); //don't want to go above the screen
	}
	if(key[SDL_SCANCODE_RETURN] == 1 && pTwo->getY() < .85){ //playerTwo
		pTwo->setY(pTwo->getY() + timeElapsed);
	}
	else if(key[SDL_SCANCODE_RSHIFT] == 1 && pTwo->getY() > -.85){ //playerTwo
		pTwo->setY(pTwo->getY() - timeElapsed);
	}
	return true;
}

void updateBallMovement(float elapsed, Entity* pOne, Entity* pTwo, Entity* ball, bool &p1, bool &p2){
	//Update the movement of the ball across the screen as it collides with boundaries and crosses goal line
	bool win = false;
	ball->setX(ball->getX() + (elapsed * ball->getdirX() * ball->getSpeed()));
	ball->setY(ball->getY() + (elapsed * ball->getdirY() * ball->getSpeed()));
	if (ball->getX() > 1.4){ //playoerOne wins a point
		p1 = true; p2 = false; win = true;
	}
	else if (ball->getX() < -1.4){
		p2 = true; p1 = false; win = true; //playertwo wins a point
	}
	if (win){
		ball->setX(0.0); //if someone won, then set the ball to the middle and reset its direction
		ball->setY(0.0);
		ball->setXDirection((-1.0) * ball->getdirX());
	}
	//check for collision with floor or ceiling
	if ((ball->getY() < -0.95) || (ball ->getY() > 0.95)) ball->setYDirection((-1.0) * ball->getdirY());
	//Checks if the ball hits either paddle
	if ((ball->getX() <-1.2f && ball->getX() > -1.5 && ball->getY() > pOne->getY() - 0.2f &&
		ball->getY() < pOne->getY() + 0.2f) || ((ball->getX() > 1.2f && ball->getX() < 1.5 && 
		ball->getY() > pTwo->getY() - 0.2f && ball->getY() < pTwo->getY() + 0.2f)))  { 
		ball->setXDirection((-1.0) * ball->getdirX());
	}	
}

void displayAndCalculate(float elapsed, Entity* pOne, Entity* pTwo, Entity* ball, bool &pOneWins, bool &pTwoWins){
	//Render the images to the screen.  The positions have already been set in the Update function.  This just draws.
	bool aPlayerWon = false;
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	pOne->drawSprite();
	pTwo->drawSprite();
	ball->drawSprite();
	//if pOne wins, change background color
	if (pOneWins){
		glClearColor(0.0, 1.0, 0.0, 0.0);
		aPlayerWon = true;
	}
	//if pTwo wins, change background color
	if (pTwoWins){
		glClearColor(1.0, 0.0, 1.0, 0.0);
		aPlayerWon = true;
	}
	if (aPlayerWon){
		glClear(GL_COLOR_BUFFER_BIT);
		pOne->drawSprite();
		pTwo->drawSprite();
		ball->drawSprite();
	}
	SDL_GL_SwapWindow(display);
}

int main(int argc, char *argv[]){
	SDL_Event event;
	Setup();
	float lastFrame = 0.0f;
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrame;
	lastFrame = ticks;
	bool pOneWins = false;
	bool pTwoWins = false;
	Entity *pOne = new Entity(-1.3, 0.0, 0.1, 0.3, LoadTexture("paddle.png"));
	pOne->setSpeed(10.0);
	Entity *pTwo = new Entity(1.3, 0.0, 0.1, 0.3, LoadTexture("paddle.png"));
	pTwo->setSpeed(10.0);
	Entity *ball = new Entity(0.0, 0.0, 0.1, 0.1, LoadTexture("ball.png"));
	ball->setXDirection(0.2);
	ball->setYDirection(-0.4);
	ball->setSpeed(5.0);
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (ProcessEvents(event, keys, pOne, pTwo, elapsed)) {
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrame;
		lastFrame = ticks;
		updateBallMovement(elapsed, pOne, pTwo, ball, pOneWins, pTwoWins);
		displayAndCalculate(elapsed, pOne, pTwo, ball, pOneWins, pTwoWins);
	}
	delete(pOne);
	delete(pTwo);
	delete(ball);
	SDL_Quit();
	return 0;
}