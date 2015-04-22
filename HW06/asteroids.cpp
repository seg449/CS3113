#include "asteroids.h"

bool debug = false;
bool livesFinished = false;
int score = 0;
int z = 0;

Asteroids::Asteroids(float timeLeft, float lastFrameTicks, bool finished){
	state = MAIN_MENU;
	this->timeLeft = timeLeft;
	this->lastFrameTicks = lastFrameTicks;
	SDL_Init(SDL_INIT_VIDEO);
	display= SDL_CreateWindow("HW06", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(display);
	SDL_GL_MakeCurrent(display, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	Matrix first;
	glLoadMatrixf(first.ml);
	sheet = LoadTexture("sheet.png");
	font = LoadTexture("font.png");
	//GLuint ID, float x, float y, float u, float v, float w, float h,
	srand (static_cast <unsigned> (time(0)));
	player = new sprite(sheet, 45.0f / 388.0f, 0.0f / 273.0f, 30.0f / 388.0f, 30.0f / 273.0f);
	player->lives = 9;
	for (size_t i = 0; i < 13; ++ i){
		sprite* temp = new sprite(sheet, 0.0f / 388.0f, 0.0f / 273.0f, 35.0f / 388.0f, 30.0f / 273.0f);
		temp->x = generateRandomFloat(-1.7f, 1.7f);
		if (debug) cout << temp->x << endl;
		temp->y = generateRandomFloat(-1.7f, 1.7f);
		if (debug) cout << temp->y << endl;
		temp->vX = generateRandomFloat(-0.5, 0.5);
		temp->vY = generateRandomFloat(-0.5, 0.5);
		temp->r = generateRandomFloat(0.0f, 6.0f);
		asteroids.push_back(temp);
	}
	
}

GLuint Asteroids::LoadTexture(const char* img){
	SDL_Surface *surface = IMG_Load(img);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA/*format*/, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void Asteroids::drawText(GLuint texture, string text, float x, float y, float size, float r, float g, float b, float a){
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;
	for (unsigned int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size) * i) + (-0.5f * size), 0.5f * size,
			((size) * i) + (-0.5f * size), -0.5f * size,
			((size) * i) + (0.5f * size), -0.5f * size,
			((size) * i) + (0.5f * size), 0.5f * size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

float Asteroids::generateRandomFloat(float LO, float HI){
	float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	return r3;
}

void Asteroids::menu(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	switch(state){
		case MAIN_MENU:
			if (debug) cout << "here" << endl;
			drawText(font, "Welcome to Asteroids", -0.95f, 0.09f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(font, "Control player and don't crash into any asteroids", -1.2f, -0.1f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(font, "You have 9 lives. Get to 10K Points!", -1.3f, -0.2f, 0.065f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(font, "Press Enter to start", -0.9f, -0.3f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case GAME:
			drawText(font, "Score: " + to_string(score) + "\tLives:" + to_string(player->lives), -0.9f, 0.9f, 0.09f, 1.0f, 1.0f, 1.0f, 1.0f);
			if (debug) cout << "here: " << z << endl;
			player->Render();
			for (size_t i = 0; i < asteroids.size(); ++i) asteroids[i]->Render();
			if (debug) ++z;
			break;
		case OVER:
			drawText(font, "Final Score: " + to_string(score), -0.9f, 0.1f, 0.07f, 1.0f, 1.0f, 1.0f, 1.0f);
			if (livesFinished){
				drawText(font, "Better Luck Next Time! Press Enter", -1.2f, -0.2f, 0.07f, 1.0f, 1.0f, 1.0f, 1.0f);
			}
			else{
				drawText(font, "Congratulations! You won!", -0.9f, -0.2f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
			}
	}
	SDL_GL_SwapWindow(display);
}


bool Asteroids::updateAndRender(){
	//return false;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			finished = true;
		}
	}
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	float fixedElapsed = elapsed + timeLeft;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeft = fixedElapsed;
	keyBoardControls(elapsed);
	menu();
	return finished;
}

void Asteroids::FixedUpdate(){
	if (state == GAME){
		score += 1;
		if (score == 10000){
			state = OVER;
			return;
		} 
		player->matrixTransformations();
		float tempX, tempY;
		tempX = float(cos(player->r + M_PI/2.0f)); 
		tempX *= player->s;
		tempY = float(sin(player->r + M_PI/2.0f));
		tempY *= player->s; 
		player->vec = Vector(tempX, tempY, 0.0f);
		player->x += player->vX * FIXED_TIMESTEP;
		if (player->x < -1.39f) player->x = 1.39f;
		else if (player->x > 1.39f) player->x = -1.39f;
		player->y += player->vY * FIXED_TIMESTEP;
		if (player->y < -1.39f) player->y = 1.25f;
		if (player->y > 1.39f) player->y = -1.25f;
		for (size_t i = 0; i < asteroids.size();++i){
			asteroids[i]->matrixTransformations();
			tempX = float(cos(asteroids[i]->r + M_PI/2.0f));
			tempX *= asteroids[i]->s;
			tempY = float(sin(asteroids[i]->r + M_PI/2.0f));
			tempY *= asteroids[i]->s;
			asteroids[i]->vec = Vector(tempX, tempY, 0.0f);
			asteroids[i]->x += asteroids[i]->vX * FIXED_TIMESTEP;
			if (asteroids[i]->x < -1.39f) {
				asteroids[i]->x = generateRandomFloat(0.0f, 1.3f);
				//below is the block of code dealing with speeding up as explained in the hw06.cpp file
				/*asteroids[i]->vX = generateRandomFloat(asteroids[i]->vX, 2 * asteroids[i]->vX);
				if (asteroids[i]->vX > 5.0f) asteroids[i]->vX = 1.0f;
				asteroids[i]->vY = generateRandomFloat(asteroids[i]->vY, 2 * asteroids[i]->vY);
				if (asteroids[i]->vY > 5.0f) asteroids[i]->vY = 1.0f;
			*/}
			else if (asteroids[i]->x > 1.39f) {
				asteroids[i]->x = generateRandomFloat(-1.3f, 0.0f);
				//below is the block of code dealing with speeding up as explained in the hw06.cpp file
				/*asteroids[i]->vX = generateRandomFloat(asteroids[i]->vX, 2 * asteroids[i]->vX);
				if (asteroids[i]->vX > 5.0f) asteroids[i]->vX = 1.0f;
				asteroids[i]->vY = generateRandomFloat(asteroids[i]->vY, 2 * asteroids[i]->vY);
				if (asteroids[i]->vY > 5.0f) asteroids[i]->vY = 1.0f;
			*/}
			asteroids[i]->y += asteroids[i]->vY * FIXED_TIMESTEP; 
			if (asteroids[i]->y < -1.39f) {
				asteroids[i]->y = generateRandomFloat(0.0f, 1.3f);
				//below is the block of code dealing with speeding up as explained in the hw06.cpp file
				/*asteroids[i]->vX = generateRandomFloat(asteroids[i]->vX, 2 * asteroids[i]->vX);
				if (asteroids[i]->vX > 5.0f) asteroids[i]->vX = 1.0f;
				asteroids[i]->vY = generateRandomFloat(asteroids[i]->vY, 2 * asteroids[i]->vY);
				if (asteroids[i]->vY > 5.0f) asteroids[i]->vY = 1.0f;
			*/}
			else if (asteroids[i]->y > 1.39f){ 
				asteroids[i]->y = generateRandomFloat(-1.3f, 0.0f);
				//below is the block of code dealing with speeding up as explained in the hw06.cpp file
				/*asteroids[i]->vX = generateRandomFloat(asteroids[i]->vX, 2 * asteroids[i]->vX);
				if (asteroids[i]->vX > 5.0f) asteroids[i]->vX = 1.0f;
				asteroids[i]->vY = generateRandomFloat(asteroids[i]->vY, 2 * asteroids[i]->vY);
				if (asteroids[i]->vY > 5.0f) asteroids[i]->vY = 1.0f;
			*/}
			for (size_t j = 0; j < asteroids.size(); ++j){
				if (!(i==j)){
					if (collisions(asteroids[i], asteroids[j], asteroids[i]->x - asteroids[j]->x, asteroids[i]->y - asteroids[j]->y, false)) {}
				}
			}
			if (collisions(player, asteroids[i], player->x - asteroids[i]->x, player->y - asteroids[i]->y, true)){
				if (debug) cout << "LIVES: " << player->lives << endl;
				if (debug) cout << "here" << endl;
				if (player->lives == 0) {
					state = OVER; 
					if (debug) cout << "here" << endl;
					livesFinished = true;
				}
			}
		}	
	}
}

bool Asteroids::collisions(sprite* s, sprite* s2, float distanceX, float distanceY, bool flag){
	if (sqrtf(pow(distanceX, 2) + pow(distanceY, 2)) - ((s->width/2) + (s2->width/2)) < 0) {
		if (flag) {
			score -= 50;
			--(player->lives);
		}
		calculateAdjustments(s, s2, distanceX, distanceY);
	}
	return flag;
}

void Asteroids::calculateAdjustments(sprite* e, sprite* e2, float distanceX, float distanceY){
	float totalLength, finalX, finalY;
	totalLength = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
	finalX = distanceX / totalLength;
	finalY = distanceY / totalLength;
	e->x += finalX * FIXED_TIMESTEP;
	e->y += finalY * FIXED_TIMESTEP;
	e2->x -= finalX * FIXED_TIMESTEP;
	e2->y -= finalY * FIXED_TIMESTEP; 
}

void Asteroids::keyBoardControls(float elapsed){
	if (debug) cout << elapsed << endl;
	const Uint8* keys = SDL_GetKeyboardState(NULL); 
	float tempX, tempY;
	tempX = float(cos(player->r + M_PI/2.0f));
	tempY = float(sin(player->r + M_PI/2.0f));
	player->vec = Vector(tempX, tempY, 0.0f);
	if (state == MAIN_MENU){
		if (keys[SDL_SCANCODE_RETURN]) state = GAME;
	}
	else if (state == GAME){
		if (keys[SDL_SCANCODE_LEFT]){
			player->r += 3.0f * elapsed;
		}
		else if (keys[SDL_SCANCODE_RIGHT]){
			player->r -= 3.0f * elapsed;
		}
		else if (keys[SDL_SCANCODE_DOWN]){
			player->x -= 0.5 * player->vec.x * FIXED_TIMESTEP;
			player->y -= 0.5 * player->vec.y * FIXED_TIMESTEP;
		}
		else if (keys[SDL_SCANCODE_UP]){
			player->x += 0.9 * player->vec.x * FIXED_TIMESTEP;
			if (debug) cout << player->x << endl;
			player->y += 0.9 * player->vec.y * FIXED_TIMESTEP;
		}
	}
	else if (state == OVER){
		if (keys[SDL_SCANCODE_RETURN]) finished = true;
	}
}