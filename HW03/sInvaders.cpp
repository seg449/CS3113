/*
Sebastien Emmanuel Gerard
CS3113
HW03
2/26/15
*/

#include "sInvaders.h"
//#include <SDL2/SDL_image.h>
//#include <initializer_list>

sInvaders::sInvaders(){
	initialize();
}

void sInvaders::initialize(){
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("HW03", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, glContext);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	setup();
	state = MAIN_MENU;
	playerWait = 0;
	enemyWait = 0;
	score = 0;
	finished = false;
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

void sInvaders::setup(){
	size = photoTexture("font1.png");
	Sprite playe = Sprite(photoTexture("sheet.png"), 0.0, -0.95, 36.0 / 55.0f, 32.0 / 55.0f, 18.0 / 55.0f, 8.0 / 55.0f, true, true);

	//create 20 aliens
	for (int i = 0; i < 20; i++) {
		enemy.insert(enemy.end(), Sprite(photoTexture("sheet.png"), -0.9f, 0.6f, 0.0 / 55.0f, 0.0f, 26.0 / 55.0f, 23.0 / 55.0f, true, true));
	}
	//this just made life easier when accessing the player
	player.push_back(playe);
	int spriteCounter = 0;
	bool row2 = true;
	for (size_t i = 0; i < enemy.size(); i++) { //make 2 rows of 10 aliens
		if (spriteCounter < 10){
			enemy[i].setx(enemy[i].getx() + spriteCounter*0.15);
			spriteCounter++;
		}
		else if (row2 && enemy[i].getVisible()){
			spriteCounter = 0;
			for (size_t i = 10; i < enemy.size(); i++) {
				enemy[i].sety(enemy[i].gety() - 0.2f);
			}
			row2 = false;
		}
	}
	for (int i = 0; i < 30; i++) { //create ammo for the players
		Sprite bullet = Sprite(photoTexture("sheet.png"), 0.0, -0.8, 46.0 / 55.0f, 8.0 / 55.0f, 10.0 / 55.0f, 5.0 / 55.0f, false, true);
		pAmmo.push_back(bullet);
	}

	for (int i = 0; i < 60; i++) { //create ammo for the enemy
		Sprite newBullet = Sprite(photoTexture("sheet.png"), 0.0, -0.8, 45.0 / 55.0f, 8.0 / 55.0f, 10.0 / 55.0f, 4.0 / 55.0f, false, true);
		eAmmo.push_back(newBullet);
	}
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

void sInvaders::menu(){ //main menu
	drawText(size, "Shoot all enemies before time ends", -0.95f, 0.09f, 0.06f, 1.0f, 1.0f, 1.0f, 1.0f);
	drawText(size, "Use left and right arrow keys to move. Enter to shoot", -0.9f, -0.2f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
	drawText(size, "Press Enter to start", -0.9f, -0.3f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);	
}

void sInvaders::render(){ //switches states throughout the game
	glClear(GL_COLOR_BUFFER_BIT);
	switch(state){
		case MAIN_MENU:
			menu();
			break;
		case PLAY_GAME:
			game();
			break;
		case OVER:
			end();
			break;
	}
	SDL_GL_SwapWindow(window);
}

bool sInvaders::update(){ //updates movement on screen
	float lastframe = 0.0f;
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	timeElapsed = ticks - lastframe;
	lastframe = ticks;
	playerWait++; enemyWait++;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			finished = true;
		}
	}
	process(); //deals with keyboard input
	playGame(); //deals with collision 
	int random = rand() % 26; //get a random enemy
	if (enemy[random].getVisible() == true)	enemyShoot(enemy[random]); //make him shoot
	for (size_t i = 0; i < eAmmo.size(); ++i){
		if (eAmmo[i].getVisible()) eAmmo[i].sety(eAmmo[i].gety() - .025f);  //get enemies to bullets to move
	}
	for (size_t i = 0; i < pAmmo.size(); ++i){
		if (pAmmo[i].getVisible()) pAmmo[i].sety(pAmmo[i].gety() + .025f); //get player bullets to move
	}
	for (size_t i = 0; i < enemy.size(); ++i){ //make enemies move 
		if ((enemy[i].getx() + .0065f) > 1.0f) { //if they reach the end of the screen
			enemy[i].setx(-1.1); //restart on the left
		}
		enemy[i].setx(enemy[i].getx() + (.0065f * enemy[i].getDirection())); //consistently moving 
	}
	for (size_t i = 0; i < eAmmo.size(); ++i){ //if bullet gets off the screen
		if (eAmmo[i].gety() < -1.0f) eAmmo[i].setVisible(false); //make it invisible
	}
	int count = 0; //count number of enemies alive at any given time
	for (int i = 0; i < enemy.size(); ++i){
		if (!(enemy[i].getStatus()) && !(enemy[i].getVisible())) ++count;
	}
	if (count == enemy.size()) state = OVER; //when all enemies are gone, game is over
	if (timeElapsed > 33) state = OVER; //if time is up, game is over
	render(); //to determine which state you are in
	return finished; //when true, exit screen
}

void sInvaders::process(){
	key = SDL_GetKeyboardState(NULL);
	float move = .0065f;
	if (state == MAIN_MENU){
		if (key[SDL_SCANCODE_RETURN]) state = PLAY_GAME; //hitting enter starts game
	}
	else if (state == PLAY_GAME){ 
		if (key[SDL_SCANCODE_RETURN]) playerShoot(player[0]); //enter allows player to shoot
		else if (key[SDL_SCANCODE_RIGHT]){ //move right
			if ((player[0].getx() + move) < 1.0f) player[0].setx(player[0].getx() + move);
		}
		else if (key[SDL_SCANCODE_LEFT]){ //move left
			if ((player[0].getx() - move) > -1.0f) player[0].setx(player[0].getx() - move);
		}

	}
	else if (state == OVER) { 
		if (key[SDL_SCANCODE_ESCAPE]) finished = true; //escape closes window
	}

}

void sInvaders::playerShoot(Sprite& player){
	if (playerWait > 50) { //don't allow a player to shoot every second 
		for (int i = 0; i < pAmmo.size(); i++) {
			if (pAmmo[i].gety() > 1.0f) { //set visibility to false if off screen
				pAmmo[i].setVisible(false);
			}
		}
		int index = 0;
		while ((pAmmo[index].getVisible()) && index < 32) {
			index++;
			if (index == 32) {
				index = 0;
			}
		}
		pAmmo[index].setVisible(true);
		pAmmo[index].setx(player.getx() + (player.getWidth() / 5));
		pAmmo[index].sety(player.gety() + 0.05f);
		playerWait = 0;
	}
}

void sInvaders::enemyShoot(Sprite& enemy){
	if (enemyWait > 80 && enemy.getVisible()){
		int ndx = 0;
		while((pAmmo[ndx].getVisible()) && ndx < 32) {
			ndx++;
			if (ndx == 32) ndx = 0;
		}
		eAmmo[ndx].setVisible(true);
		eAmmo[ndx].setx(enemy.getx());
		eAmmo[ndx].sety(enemy.gety() - 0.05f);
		enemyWait = 0;
	} 
}

void sInvaders::game(){
	drawText(size, to_string(score) + "               Time:" + to_string(33-timeElapsed), -0.9f, 0.9f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
	player[0].drawSprite(0.2f);
	for (size_t i = 0; i < enemy.size(); ++i){
		if (enemy[i].getVisible()) enemy[i].drawSprite(0.2f);
	}
	for (size_t i = 0; i < pAmmo.size(); ++i){
		if (pAmmo[i].getVisible()) pAmmo[i].drawSprite(0.7f); 
	}
	for (size_t i = 0; i < eAmmo.size(); ++i){
		if (eAmmo[i].getVisible()) eAmmo[i].drawSprite(0.6f);
	}
}

void sInvaders::end(){
	glClear(GL_COLOR_BUFFER_BIT);
	drawText(size, "GAME OVER", -0.8f, 0.4f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f);
	drawText(size, "SCORE: " + to_string(score), -0.8f, 0.0f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
	drawText(size, "Press ESC", -0.8f, -0.4f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void sInvaders::playGame(){
	for (size_t i = 0; i < pAmmo.size(); ++i){
		for (size_t j = 0; j < enemy.size(); ++j){ //deals with collision between enemy and bullet
			if (((pAmmo[i].getx() > enemy[j].getx() && pAmmo[i].getx() < enemy[j].getx() + .35*enemy[j].getWidth()) ||
				(pAmmo[i].getx() + .35*pAmmo[i].getWidth() < enemy[j].getx() + .35*enemy[j].getWidth()
				&& pAmmo[i].getx() + .35*pAmmo[i].getWidth() > enemy[j].getx())) && ((pAmmo[i].gety() > enemy[j].gety()
				&& pAmmo[i].gety() < enemy[j].gety() + .35*enemy[j].getHeight()) ||
				(pAmmo[i].gety() + .35*pAmmo[i].getHeight() < enemy[j].gety() + .35*enemy[j].getHeight()
				&& pAmmo[i].gety() + .35*pAmmo[i].getHeight() > enemy[j].gety())) && pAmmo[i].getVisible() && enemy[j].getVisible()) {
					pAmmo[i].setVisible(false);
					enemy[j].setVisible(false);
					enemy[j].setStatus(false);
					score += 50;
					j--;
			}
		}
	}
	for (size_t i = 0; i < eAmmo.size(); ++i){ //collision between enemmy bullet and player
		if (((eAmmo[i].getx() > player[0].getx() && eAmmo[i].getx() < player[0].getx() + .1*player[0].getWidth()) ||
			(eAmmo[i].getx() + .1*eAmmo[i].getWidth() < player[0].getx() + .1*player[0].getWidth()
			&& eAmmo[i].getx() + .1*eAmmo[i].getWidth() > player[0].getx())) && ((eAmmo[i].gety() > player[0].gety()
			&& eAmmo[i].gety() < player[0].gety() + .1*player[0].getHeight()) ||
			(eAmmo[i].gety() + .1*eAmmo[i].getHeight() < player[0].gety() + .1*player[0].getHeight()
			&& eAmmo[i].gety() + .1*eAmmo[i].getHeight() > player[0].gety())) && eAmmo[i].getVisible() && player[0].getVisible()) {
			state = OVER;
		}
	}
}