/*
Sebastien Emmanuel Gerard
CS3113
HW03
2/26/15
*/

//I got most of the features working. The main bug in the program is that I couldn't get the collision between enemies and player bullets working
//Hence, the score remains at 0. Also, the collision between the player and the enemy's bullets only works sometimes and not all the time. 


using namespace std;

#include "sprite.h"
#include "sInvaders.h"

int main(){
	//Sprite* player = new Sprite(sInvaders::photoTexture("sheet.png"), 0.0, -0.95, 36.0 / 55.0f, 32.0 / 55.0f, 18.0 / 55.0f, 8.0 / 55.0f, true, true);
	//std::vector<float> v;
	//v.push_back(0);
	sInvaders sI(0.0, 0.0, 0, false);
	while (!sI.playGame()){
		//bool status = sI.update();
		//if (status == true) break;
	}
	return 0;
}