/*
Sebastien Emmanuel Gerard
CS3113
HW03
2/26/15
*/

#include "sInvaders.h"

sInvaders::sInvaders(float playerTime, float enemyTime, int score, bool finished) : player(photoTexture("sheet.png"), 0.0, -0.95, 36.0 / 55.0f, 32.0 / 55.0f, 18.0 / 55.0f, 8.0 / 55.0f, true){
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("HW03", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	size = photoTexture("font.png");
	for (int i = 0; i < 13; i++) {
		Sprite temp = Sprite(photoTexture("sheet.png"), -0.9f, 0.6f, 0.0, 0.0f, 26.0 / 55.0f, 23.0 / 55.0f, true);
		enemy.push_back(temp);
	}
	for (size_t i = 0; i < enemy.size(); i++) { 
		enemy[i].setx(enemy[i].getx() + i*0.14);
	}
	for (int i = 0; i < 100; i++) { //create ammo for the players
		Sprite bullet = Sprite(photoTexture("sheet.png"), 0.0, -0.8, 46.0 / 55.0f, 8.0 / 55.0f, 2.0 / 11.0f, 1.0 / 11.0f, false);
		pAmmo.push_back(bullet);
	}

	for (int i = 0; i < 200; i++) { //create ammo for the enemy
		Sprite newBullet = Sprite(photoTexture("sheet.png"), 0.0, -0.8, 9.0 / 11.0f, 8.0 / 55.0f, 2.0 / 11.0f, 4.0 / 55.0f, false);
		eAmmo.push_back(newBullet);
	}
	count = 0;
	ndx = 0;
	index2 = 0;
	playerWait = playerTime;
	enemyWait = enemyTime;
	finished = finished;
	state = MAIN_MENU;
	score = score;
}

GLuint sInvaders::photoTexture(const char* img){
	SDL_Surface* surface = IMG_Load(img);
	if (!surface) cout << "error" << endl;
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return id;
}


//drawtext function is copied from slides. Just edited the for loop a little 
void sInvaders::drawText(GLuint texture, string word, float x, float y, float s, float r, float g, float b, float a){
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

void sInvaders::render(){ //switches states throughout the game
	glClear(GL_COLOR_BUFFER_BIT);
	switch(state){
		case MAIN_MENU:
			drawText(size, "Shoot all enemies before time ends", -0.95f, 0.09f, 0.06f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "Use left and right arrow keys to move. Enter to shoot", -0.9f, -0.2f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "Press Enter to start", -0.9f, -0.3f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case PLAY_GAME:
			drawText(size, to_string(score) + "\tTime:" + to_string(40-timeElapsed), -0.9f, 0.9f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
			player.drawSprite(0.2f);
			for (size_t i = 0; i < enemy.size(); ++i){
				if (enemy[i].getVisible()) enemy[i].drawSprite(0.2f);
			}
			for (size_t i = 0; i < pAmmo.size(); ++i){
				if (pAmmo[i].getVisible()) pAmmo[i].drawSprite(0.7f); 
			}
			for (size_t i = 0; i < eAmmo.size(); ++i){
				if (eAmmo[i].getVisible()) eAmmo[i].drawSprite(0.6f);
			}
			break;
		case OVER:
			drawText(size, "GAME OVER", -0.8f, 0.4f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "SCORE: " + to_string(score), -0.8f, 0.0f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
			drawText(size, "Press ESC", -0.8f, -0.4f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
			break;
	}
	SDL_GL_SwapWindow(window);
}

bool sInvaders::playGame(){ //updates movement on screen
	//state = MAIN_MENU;
	float lastframe = 0.0f;
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	timeElapsed = ticks - lastframe;
	lastframe = ticks;
	playerWait++; enemyWait++;
	
	keyboardInput(); //deals with keyboard input
	if (ndx == 13) ndx = 0; 
	if ((enemy[ndx++].getVisible()) && enemyWait > 80) {
		eAmmo[ndx].setVisible(true);
		eAmmo[ndx].setx(enemy[ndx].getx()); eAmmo[ndx].sety(enemy[ndx].gety());
		enemyWait = 0;
	}
	
	count = 0; //count number of enemies alive at any given time
	for (int i = 0; i < enemy.size(); ++i){
		if (!(enemy[i].getStatus()) && !(enemy[i].getVisible())) ++count;
		if ((enemy[i].getx() + .008f) > 1.0f) { //if they reach the end of the screen
			enemy[i].setx(-1.1); //restart on the left
		}
		enemy[i].setx(enemy[i].getx() + (.008f * enemy[i].getDirection())); //consistently moving
	}
	for (size_t i = 0; i < eAmmo.size(); ++i){
		if (eAmmo[i].getVisible()) eAmmo[i].sety(eAmmo[i].gety() - .035f);  //get enemies to bullets to move
		if (eAmmo[i].gety() < -1.0f) eAmmo[i].setVisible(false);
	}
	checkCollisions(); //deals with collision 
	for (size_t i = 0; i < pAmmo.size(); ++i){
		if (pAmmo[i].getVisible()) pAmmo[i].sety(pAmmo[i].gety() + .035f); //get player bullets to move
	}
	if (count == enemy.size()) state = OVER; glClear(GL_COLOR_BUFFER_BIT); //when all enemies are gone, game is over
	if (timeElapsed > 40) state = OVER; glClear(GL_COLOR_BUFFER_BIT); //if time is up, game is over
	render(); //to determine which state you are in
	return finished; //when true, exit screen
}

void sInvaders::keyboardInput(){
	const Uint8* key = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			finished = true;
		}
	}
	if (state == MAIN_MENU){
		if (key[SDL_SCANCODE_RETURN]) state = PLAY_GAME; //hitting enter starts game
	}
	else if (state == PLAY_GAME){ 
		if (key[SDL_SCANCODE_RETURN] && playerWait > 50) {//enter allows player to shoot
			for (int i = 0; i < pAmmo.size(); i++) {
				if (pAmmo[i].gety() > 1.0f) pAmmo[i].setVisible(false); //set visibility to false if off screen
			}
			index2 = 0;
			while (pAmmo[index2++].getVisible()) {}
			pAmmo[index2].setVisible(true);
			pAmmo[index2].setx(player.getx());
			pAmmo[index2].sety(player.gety());
			playerWait = 0;
		}
		else if (key[SDL_SCANCODE_RIGHT] && (player.getx() + .007f) < 1.0f) { //move right
			player.setx(player.getx() + .007);
		}
		else if (key[SDL_SCANCODE_LEFT] && (player.getx() - .007f) > -1.0f){ //move left
			player.setx(player.getx() - .007f);
		}
	}
	else if (state == OVER) { 
		if (key[SDL_SCANCODE_ESCAPE]) finished = true; //escape closes window
	}
	//cout << "player position after set: " << player->getx() << endl;
}

void sInvaders::checkCollisions(){
	//I was only able to get the collision partially working (not completely working) between the enemy's bullet and the player. 
	for (size_t i = 0; i < eAmmo.size(); ++i){ //collision between enemmy bullet and player
		if (((eAmmo[i].getx() > player.getx() && eAmmo[i].getx() < player.getx() + player.getWidth()) ||
			(eAmmo[i].getx() + eAmmo[i].getWidth() < player.getx() + player.getWidth()
			&& eAmmo[i].getx() + eAmmo[i].getWidth() > player.getx())) && ((eAmmo[i].gety() > player.gety()
			&& eAmmo[i].gety() < player.gety() + player.getHeight())) && eAmmo[i].getVisible() && player.getVisible()) {
			state = OVER;
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
}