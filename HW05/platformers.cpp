#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define TILE_SIZE 0.08f
#define WORLD_OFFSET_X 0.0f
#define WORLD_OFFSET_Y 0.0f

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
	window = SDL_CreateWindow("HW05", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
	bombSound = Mix_LoadWAV("bomb.wav");
	coinS = Mix_LoadWAV("coin.wav");
	music = Mix_LoadMUS("music.wav");
	sheet = photoTexture("sheet.png");
	size = photoTexture("font.png");
	//renderObjects();
	ifstream infile("mymap.txt");
	if (!infile) cerr << "err" << endl;
	string line;
	while (getline(infile, line)){
		if (debug) cout << "Here" << endl;
		if (line == "[header]"){
			if (!readHeader(infile)) {
				if (debug) cout << "header" << endl;
				return;
			}
		}
		else if (line == "[layer]"){
			if (debug) cout << "layer" << endl;
			readLayerData(infile); 
		}
		else if (line == "[Object]"){
			if (debug) cout << "object" << endl;
			readEntityData(infile);
		}
	}
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

bool platformers::readHeader(ifstream& stream){
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { 
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool platformers::readLayerData(ifstream& stream){
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 12;
					}
				}
			}
		}
	}
	return true;
}

bool platformers::readEntityData(ifstream& stream){
	string line, type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16.0f * .08f + .08f;
			float placeY = atoi(yPosition.c_str()) / 16.0f * -.08f;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void platformers::placeEntity(string type, float placeX, float placeY){
	if (type == "p1"){
		player = new Sprite(placeX, placeY);
		player->id = sheet;
		player->u = (float)(98 % 16) / 16.0f;
		player->v = (float)(98 / 16) / 8.0f;
		player->width = 1.0f / 16.0f;
		player->height = 1.0f / 8.0f;
	}
	else if (type == "bomb"){
		Sprite* bomb = new Sprite(placeX, placeY);
		bomb->id = sheet;
		bomb->u = (float)(62 % 16) / (float)16.0;
		bomb->v = (float)(62 / 16) / (float)8.0;
		bomb->width = 1.0f / 16.0f;
		bomb->height = 1.0f / 8.0f;
		bombs.push_back(bomb);
	}
	else if (type == "key"){
		Sprite* key = new Sprite(placeX, placeY);
		key->id = sheet;
		key->u = (float)(86 % 16) / (float)16.0;
		key->v = (float)(86 / 16) / (float)8.0;
		key->width = 1.0f / 16.0f;
		key->height = 1.0f / 8.0f; 
		regObjects.push_back(key);
	}
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
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(-player->x, -player->y, 0.0f);
			renderMap();
			if (player->isVisible){
				player->Render(-.08f * mapWidth/2, .08f * mapHeight/2);
			}
			for (size_t i = 0; i < regObjects.size(); ++i){
				if (regObjects[i]->isVisible){
					regObjects[i]->Render(-.08f * mapWidth/2, .08f * mapHeight/2);
				}
				else {++temp;}
			}
			for (size_t i = 0; i < bombs.size(); ++i){
				if (bombs[i]->isVisible){
					bombs[i]->Render(-.08f * mapWidth/2, .08f * mapHeight/2);
				}
			}
			if (temp == regObjects.size()) {
				glClearColor(0.0, 0.0, 0.0, 0.0);
				state = OVER;
			}
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glPopMatrix();
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

float lerp(float v0, float v1, float t){ 
	return (1.0f - t) * v0 + t * v1;
}

void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)((worldX + (WORLD_OFFSET_X)) / 0.08f);
	*gridY = (int)((-worldY + (WORLD_OFFSET_Y)) / 0.08f);
}

bool platformers::leftSpriteCollision(Sprite* s, float* newVal){
	//cout << "LEFT" << endl;
	//check left side
	int gridX, gridY; 
	//float newVal;
	worldToTileCoordinates(s->x - s->width/2, s->y, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight){
		return false;
	}
	if (levelData[gridY][gridX] == 1 || levelData[gridY][gridX] == 32){
		float offset = gridX * .08f;
		*newVal = (-(s->x - s->width/2) - offset);
		return true;
	}
	*newVal = 0.0f;
	return false;
}

bool platformers::rightSpriteCollision(Sprite* s, float* newVal){
	int gridX, gridY; 
	worldToTileCoordinates(s->x + s->width/2, s->y, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight){
		return false;
	}
	if (levelData[gridY][gridX] == 1 || levelData[gridY][gridX] == 32){
		float offset = gridX * .08f;
		*newVal = (-(s->x + s->width/2) - offset);
		return true;
	}
	*newVal = 0.0f;
	return false;
}

bool platformers::bottomSpriteCollision(Sprite* s, float* newVal){
	//cout << "BOTTOM5" << endl;
	int gridX, gridY;
	float val = s->y - (s->height / 2);
	if (debug) cout << val << endl;
	worldToTileCoordinates(s->x, val, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > 32){
		*newVal = 0.0;
		return false;
	}
	if (levelData[gridY][gridX] == 1 || levelData[gridY][gridX] == 32){
		float offset = gridY * .08f;
		*newVal = (-(s->y - s->height / 2) - offset);
		return true;
	}
	*newVal = 0.0f;
	return false;
}	

bool platformers::topSpriteCollision(Sprite* s, float* newVal){
	//cout << "TOP" << endl;
	int gridX, gridY;
	worldToTileCoordinates(s->x, s->y + s->height / 2, &gridX, &gridY);
	if (gridX < 0 || gridX > 64 || gridY < 0 || gridY > 32){
		return false;
	}
	if (levelData[gridY][gridX] == 1 || levelData[gridY][gridX] == 32){
		float offset = gridY * .08f;
		*newVal = (-(s->y + s->height / 2) - offset);
		return true;
	}
	*newVal = 0.0f;
	return false;
}

void platformers::FixedUpdate(){ 
	Mix_PlayMusic(music, -1);
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
			
		if (debug) cout << player->velocity_x << "\t" << player->acceleration_x << endl;
		if (!player->collidedBottom) {player->acceleration_y = -1.8;}
		
		//touching a bomb, ends the game
		for (size_t i = 0; i < bombs.size(); ++i){
			if (player->collidesWith(bombs[i])){
				Mix_PlayChannel(-1, bombSound, 0);
				state = OVER;
			}
		}
		for (size_t i = 0; i < regObjects.size(); ++i){
			if (player->collidesWith(regObjects[i])){
				regObjects[i]->isVisible = false;
				Mix_PlayChannel(-1, coinS, 0);
			}
		}
		float newVal = 0.0f;
		if (leftSpriteCollision(player, &newVal)){
			player->x -= newVal * .08f * FIXED_TIMESTEP;
			player->velocity_x = 0.0f;
		}
		if (rightSpriteCollision(player, &newVal)){
			player->x += newVal * .08f * FIXED_TIMESTEP;
			player->velocity_x = 0.0f;
		}
		if (topSpriteCollision(player, &newVal)){
			player->y += newVal - .08f;
			player->velocity_y = 0.0f;
		}
		if (bottomSpriteCollision(player, &newVal)){
			player->y -= newVal + .08f;
			player->velocity_y = 0.0f;
			player->collidedBottom = true;
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

void platformers::renderMap(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sheet);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	vector<float> vertexData;
	vector<float> texCoordData;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			float u = (float)(((int)levelData[y][x]) % 16) / 16.0f;
			float v = (float)(((int)levelData[y][x]) / 16) / 8.0f;
			float spriteWidth = 1.0f / 16.0f;
			float spriteHeight = 1.0f / 8.0f;
			vertexData.insert(vertexData.end(), {
				.08f * x, -.08f * y,
				.08f * x, (-.08f * y) - .08f,
				(.08f * x) + .08f, (-.08f * y) - .08f,
				(.08f * x) + .08f, -.08f * y
			});
			texCoordData.insert(texCoordData.end(), { u, v,
				u, v + (spriteHeight),
				u + spriteWidth, v + (spriteHeight),
				u + spriteWidth, v
			});
		}
	}
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, mapHeight*mapWidth * 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}