#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
float timeLeftOver = 0.0f;

#include "platformers.h"

bool debug = false; //used when I was debugging my code. 

platformers::platformers(int score, bool finished, float lfTicks, float timeElapsed){
	state = MAIN_MENU;
	score = score;
	finished = finished;
	lfTicks = lfTicks;
	timeElapsed = timeElapsed;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("HW04", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	sheet = photoTexture("sheet1.png");
	size = photoTexture("font.png");
	renderObjects();
}

void platformers::renderObjects(){
	//right side
	for (float height = -.95f; height < 1.01f; height += .103f){
		Sprite* block = new Sprite(sheet, 1.3f, height, 0.0 / 314.0f, 0.0 / 281.0f, 70.0 / 314.0f, 75.0 / 281.0f);
		solidObjects.push_back(block);
	}
	//left side 
	for (float height = -.95f; height < 1.01f; height += .103f){
		Sprite* block = new Sprite(sheet, -1.3f, height, 0.0 / 314.0f, 0.0 / 281.0f, 70.0 / 314.0f, 75.0 / 281.0f);
		solidObjects.push_back(block);
	}
	//bottom
	for (float width = -1.4f; width < 1.4f; width += .086f){
		Sprite* block = new Sprite(sheet, width, -.95f, 0.0 / 314.0f, 0.0 / 281.0f, 70.0 / 314.0f, 75.0 / 281.0f);
		solidObjects.push_back(block);
	}
	//1st level
	for (float width = -.85; width < .2; width += .086f){ 
		Sprite* block = new Sprite(sheet, width, -.5f, 0.0 / 314.0f, 0.0 / 281.0f, 70.0 / 314.0f, 75.0 / 281.0f);
		solidObjects.push_back(block);
	}
	//2nd level
	for (float width = 0.0f; width < .6f; width += .086f){ 
		Sprite* block = new Sprite(sheet, width, -.15f,0.0 / 314.0f, 0.0 / 281.0f, 70.0 / 314.0f, 75.0 / 281.0f);
		solidObjects.push_back(block);
	}
	//3rd level
	for (float width = -1.2; width < 0.0f; width += .086f){ 
		Sprite* block = new Sprite(sheet, width, .2f, 0.0 / 314.0f, 0.0 / 281.0f, 70.0 / 314.0f, 75.0 / 281.0f);
		solidObjects.push_back(block);
	}
	player = new Sprite(sheet, 0.0f, 0.0f, 0.0 / 314.0f, 75.0 / 281.0f, 110.0 / 314.0f, 140.0 / 281.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, false);
	regObjects.push_back(new Sprite(sheet, 0.0f, 0.7f, 167.0 / 314.0f, 0.0 / 281.0f, 100.0 / 314.0f, 75.0 / 281.0f));
	regObjects.push_back(new Sprite(sheet, 1.0f, 0.7f, 167.0 / 314.0f, 0.0 / 281.0f, 100.0 / 314.0f, 75.0 / 281.0f));
	regObjects.push_back(new Sprite(sheet, 0.5f, 0.7f, 167.0 / 314.0f, 0.0 / 281.0f, 100.0 / 314.0f, 75.0 / 281.0f));
	regObjects.push_back(new Sprite(sheet, -0.5f, 0.7f, 167.0 / 314.0f, 0.0 / 281.0f, 100.0 / 314.0f, 75.0 / 281.0f));
	regObjects.push_back(new Sprite(sheet, -1.0f, 0.7f, 167.0 / 314.0f, 0.0 / 281.0f, 100.0 / 314.0f, 75.0 / 281.0f));
}

void platformers::menu(){
	glClear(GL_COLOR_BUFFER_BIT);
	int temp = 0;
	switch(state){
		case MAIN_MENU:
			drawText(size, "Collect all the items ", -0.95f, 0.09f, 0.06f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "Use left and right arrow keys to move. Up to Jump", -0.9f, -0.2f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "Press Enter to start", -0.9f, -0.3f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case GAME:
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glLoadIdentity();
			if (player->isVisible){
				player->Render();
			}
			for (size_t i = 0; i < solidObjects.size(); ++i){
				solidObjects[i]->Render();
			}
			
			for (size_t i = 0; i < regObjects.size(); ++i){
				if (regObjects[i]->isVisible){
					regObjects[i]->Render();
				}
				else {++temp;}
			}
			if (temp == regObjects.size()) {
				glClearColor(0.0, 0.0, 0.0, 0.0);
				state = OVER;
			}
			break;
		case OVER:
			glClearColor(0.0, 0.0, 0.0, 0.0);
			drawText(size, "GAME OVER", -0.8f, 0.4f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "CONGRATULATIONS! YOU WON!", -0.8f, 0.0f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "Press ESC", -0.8f, -0.4f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
			break;
	}
	SDL_GL_SwapWindow(window);
}

GLuint platformers::photoTexture(const char* img){
	SDL_Surface* surface = IMG_Load(img);
	if (!surface) cout << "error" << endl;
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return id;
}

void platformers::drawText(GLuint texture, string word, float x, float y, float s, float r, float g, float b, float a){
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	vector<float> vertexData;
	vector<float> textCoordData;
	vector<float> colorData;
	float textureSize = 1.0 / 16.0f;
	for (int i = 0; i <word.size(); i++) {
		float textX = (float)(((int)word[i]) % 16) / 16.0f;
		float textY = (float)(((int)word[i]) / 16) / 16.0f;
		vertexData.insert(
			vertexData.end(), { s*i + -1 * s, s, s*i + -1 * s, -1 * s, s*i + s, -1 * s, s*i + s, s });
		colorData.insert(
			colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		textCoordData.insert(
			textCoordData.end(), { textX, textY, textX, textY + textureSize, textX + textureSize, textY + textureSize, textX + textureSize, textY });
	}
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, word.size() * 4.0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void platformers::keyboardControl(float elapsed){
	const Uint8* key = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			finished = true;
		}
	}	
	if (key[SDL_SCANCODE_RETURN] && state == MAIN_MENU) state = GAME;
	else if (state == GAME){
		if (debug) cout << "KEYBOARD FUNCTION: " << player->collidedBottom << endl;
		if (key[SDL_SCANCODE_RIGHT]) player->acceleration_x = 1.0f;
		else if (key[SDL_SCANCODE_LEFT]) player->acceleration_x = -1.0f;
		else if (key[SDL_SCANCODE_UP] && player->collidedBottom) {
			player->velocity_y = 2.0f; 
			player->collidedBottom = false; 
			if (debug) cout << "UP" << endl;
		}
	}
	else if (state == OVER) { 
		if (key[SDL_SCANCODE_ESCAPE]) finished = true; //escape closes window
	}
}
//check if there is a collision on the bottom
bool didCollideBottom(Sprite* s, Sprite* s2){
	if (s->y - s->height*0.2f < s2->y + s2->height*0.2f && s->y - s->height*0.2f > s2->y - s2->height*0.2f &&
		((s->x - s->width*0.2f < s2->x + s2->width*0.2f&&s->x - s->width*0.1f > s2->x - s2->width*0.2f) ||
		(s->x + s->width*0.2f > s2->x - s2->width*0.2f&&s->x + s->width*0.1f < s2->x + s2->width*0.2f))){
		s->collidedBottom = true;
		return true;
	}
	return false;
}
//check if there is a collision on the top 
bool didCollideTop(Sprite* s, Sprite* s2){ //code is same as above with just a couple of changes from '+' to '-'
	if (s->y + s->height * .2f < s2->y + s2->height * .2f && s->y + s->height * .2f > s2->y - s2->height * .2f &&
		((s->x + s->width * .2f > s2->x - s2->width*.2f && s->x + s->width * .1f < s2->x + s2 ->width *.2f) || 
		(s->x - s->width * .2f < s2->x + s2->width * .2f && s->x -s->width * .1f > s2->x - s2->width*.2f))){  
		s->collidedTop = true;
		return true;
	}
	return false;
}
//check if there is a collision on the left
bool didCollideLeft(Sprite* s, Sprite* s2){ 
	if (s->x - s->width*0.2f < s2->x + s2->width*0.2f&&s->x - s->width*0.2f > s2->x - s2->width*0.2f &&
		((s->y - s->height*0.1f < s2->y + s2->height*0.2f&&s->y - s->height*0.1f > s2->y - s2->height*0.2f) ||
		(s->y + s->height*0.1f < s2->y + s2->height*0.2f&&s->y + s->height*0.1f > s2->y - s2->height*0.2f))){
		s->collidedLeft = true;
		return true;
	}
	return false;
}
//check if there is a collision on the right
bool didCollideRight(Sprite* s, Sprite* s2){
	//I wasn't able to get this function to completely work so I some what worked around it. For some reason, everytime I collided
	//on the right, the y position would get set and I had no idea what was going on. I commented out below the code below that I would have
	//originally used for testing for the right collision but I am not using it.  
	if (s->x + s->width * .2f < s2->x + s2->width * .2f && s->x +s->width * .1f > s2->x - s2->width*.2f &&
	   ((s->y + s->height * .2f < s2->y + s2->height * .2f && s->y + s->height * .2f > s2->y - s2->height * .2f) || 
	   	(s->y - s->height * .2f < s2->y + s2->height * .2f && s->y - s->height * 0.2f > s2->y - s2->height * 0.2f))){ 
		s->collidedRight = true;
		return true;
	}
	return false;
}

float lerp(float v0, float v1, float t){ 
	return (1.0f - t) * v0 + t * v1;
}

void platformers::FixedUpdate(){ 
	if (state == GAME){
		if (debug) cout << FIXED_TIMESTEP << endl;
		player->velocity_x = lerp(player->velocity_x, 0.0f, FIXED_TIMESTEP * player->friction_x);
		if (debug) cout << "BEFORE: " << player->velocity_x  << "\t" << player->x << endl;
		player->velocity_x += player->acceleration_x * FIXED_TIMESTEP;
		if (debug) cout << "AFTER: " << player->velocity_x << "\t" << player->x << endl;
		player->x += player->velocity_x * FIXED_TIMESTEP;
		player->velocity_y = lerp(player->velocity_y, 0.0f, FIXED_TIMESTEP * player->friction_y);
		player->velocity_y += player->acceleration_y * FIXED_TIMESTEP;
		player->y += player->velocity_y * FIXED_TIMESTEP;
	
		if (player->x < -1.2f) player->x = -1.2f;
		//if (player->y < -0.8f) {player->y = -0.8f; player->collidedBottom = true;}
			
		if (debug) cout << player->velocity_x << "\t" << player->acceleration_x << endl;
		if (!player->collidedBottom) {player->acceleration_y = -1.8;}
		if (debug) cout << "SIZE: " << solidObjects.size() << endl;
		for (size_t i = 0; i < solidObjects.size(); ++i){
			/*if (player->collidesWith(solidObjects[i])){
				if (debug) cout << "here" << endl;
				if (player->x < solidObjects[i]->x){
					if (debug) cout << "FIRST" << endl;
					player->x = solidObjects[i]->x + solidObjects[i]->width * 0.2f + player->width * 0.2f;
					player->velocity_x = 0.0f;
				}
				else if (player->x > solidObjects[i]->x){
					if (debug) cout << "SECOND" << endl;
					player->x = solidObjects[i]->x - solidObjects[i]->width * 0.2f - player->width * 0.2f;
					player->velocity_x = 0.0f;
				}
				if (player->y > solidObjects[i]->y){
					if (debug) cout << "THIRD" << endl;
					player->y = solidObjects[i]->y + solidObjects[i]->height*0.2f + player->height*0.2f;
					if (player->y > 0.9f) player->y = 0.9f; 
					player->velocity_y = 0.0f;
				}
				else if (player->y < solidObjects[i]->y){
					if (debug) cout << "FOURTH" << endl;
					player->y = solidObjects[i]->y - solidObjects[i]->height * 0.2f - player->height * 0.2f;
					player->velocity_y = 0.0f;
				}
			}*/

			//I attempted to just use the collides with function as shown above, but it didn't work out to well so I just added 
			//four boolean functions to check collisions on each side. The collision with the right wall is
			//a little funny and I have no idea how to fix it. For some reason, any collision with the right wall
			//increases the y value of the player. 

			if (debug) cout << "FixedUpdate function: " << player->collidedBottom << endl;
			if (didCollideRight(player, solidObjects[i])){
				player->x = solidObjects[i]->x - solidObjects[i]->width * 0.2f - player->width * 0.2f;
				if (debug) cout << "Collide Right" << endl;
				player->velocity_x = 0.0f;
				player->collidedRight = false;
			}
			if (player->x > 1.2f) player->x = 1.2f;
			else if (didCollideLeft(player, solidObjects[i])){
				player->x = solidObjects[i]->x + solidObjects[i]->width * 0.2f + player->width * 0.2f;
				if (debug) cout << "Collide Left" << endl;
				player->velocity_x = 0.0f;
				//player->collidedLeft = false;
			}
			else if (didCollideTop(player, solidObjects[i])){
				if (debug) cout << "Collide Top" << endl;
				player->y = solidObjects[i]->y - solidObjects[i]->height * 0.2f - player->height * 0.2f;
				player->velocity_y = 0.0f;
				player->collidedTop = false;
			}
			else if (didCollideBottom(player, solidObjects[i])){
				player->y = solidObjects[i]->y + solidObjects[i]->height*0.2f + player->height*0.2f;
				if (player->y > 0.9f) player->y = 0.9f; 
				if (debug) cout << player->y << endl;
				player->velocity_y = 0.0f;
				//player->collidedBottom = false;
			}
			
		}
		for (size_t i = 0; i < regObjects.size(); ++i){
			if (didCollideRight(player, regObjects[i])){
				regObjects[i]->isVisible = false;
				player->collidedRight = false;
			}
			else if (didCollideLeft(player, regObjects[i])){
				regObjects[i]->isVisible = false;
				player->collidedLeft = false;
			}
			else if (didCollideTop(player, regObjects[i])){
				regObjects[i]->isVisible = false;
				player->collidedTop = false;
			}
			else if (didCollideBottom(player, regObjects[i])){
				regObjects[i]->isVisible = false;
				player->collidedBottom = false;
			}
		}
	}
}

bool platformers::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	timeElapsed = ticks - lfTicks;
	lfTicks = ticks;

	float fixedTimeElapsed = timeElapsed + timeLeftOver;
	if (fixedTimeElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedTimeElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedTimeElapsed >= FIXED_TIMESTEP){
		fixedTimeElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedTimeElapsed;
	keyboardControl(timeElapsed);
	menu();
	return finished;
}