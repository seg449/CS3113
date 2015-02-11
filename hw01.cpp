/*
Sebastien Emmanuel Gerard
HW01
2/9/2015
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <iostream>

SDL_Window* display;
//this function was taken directly from the slides.  
void DrawSprite(GLint texture, float x, float y, float rotation) { 
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D, texture); 

	glMatrixMode(GL_MODELVIEW); 

	glLoadIdentity(); 
	glTranslatef(x, y, 0.0); 
	glRotatef(rotation, 0.0, 0.0, 1.0); 
 
	GLfloat quad[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f}; 
	glVertexPointer(2, GL_FLOAT, 0, quad); 
	glEnableClientState(GL_VERTEX_ARRAY); 
 
	GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0}; 
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs); 
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	 
	glDrawArrays(GL_QUADS, 0, 4); 
	glDisable(GL_TEXTURE_2D);
}

GLuint createPhotoTexture(const char* img){
	SDL_Surface* image = IMG_Load(img);
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SDL_FreeSurface(image);
	return id;
}

void drawQuads(){
	//Begin to draw
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Initialize the Quad 
	glBegin(GL_QUADS);
	glColor3f(.5, 1, 0);
	glVertex3f(-1.0, -1.0, 0.0);
	glColor3f(0, 1, 0);
	glVertex3f(-0.5, 1.8, 0.0);
	glColor3f(0.5, 0.5, 1);
	glVertex3f(0.5, 1.2, 0.0);
	glColor3f(1, 1, 1);
	glVertex3f(0.5, -0.5, 0.0);
	glEnd();
}

void makeAnimation(int wid, int height, SDL_Event event){
	//get all the variables initialized first
	bool done = false;
	SDL_Init(SDL_INIT_VIDEO);
	display = SDL_CreateWindow("HW01", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		wid, height, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(display);
	SDL_GL_MakeCurrent(display, glContext);
	//once the initialization is done, create the viewport
	glViewport(0, 0, wid, height);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	float frame, r, s;
	frame = 0.0f;
	r = 0.0f; //rotation
	s = 0.0f; //speed
	//once the viewport is done, load all the images (3)
	GLuint iniesta = createPhotoTexture("iniesta.jpg");
	GLuint isco = createPhotoTexture("isco.jpg");
	GLuint dimaria = createPhotoTexture("dimaria.jpg");

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		//Take care of the animation
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - frame;
		frame = ticks;
		r += 60.f * elapsed;
		s += 1.5f * elapsed;
		if (s > 1.0f){
			s = -1.2f;
		}
		drawQuads();
		//Draw the images
		DrawSprite(iniesta, 0.2, 0.3, r* 2.0f);                                                                   //outside of school.
		DrawSprite(isco, 1.2, 0.3, r* 2.0f);
		DrawSprite(dimaria, s, -0.6, r*1.0f);
		SDL_GL_SwapWindow(display);
	}
	SDL_Quit();
}

int main(int argc, char* argv[]){
	int wid = 500;
	int height = 500;
	SDL_Event event;
	makeAnimation(wid, height, event);
}



