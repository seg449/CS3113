//Sebastien Emmanuel Gerard
//3/28/15
//HW04
//Per our discussion via email, thank you for allowing me an extension on this assignment. I apologize for handing it in so late. 

#include "platformers.h"


//For the most part it works. My only error is that when the player collides with the right wall, the y value of the player just changes and I wasn't sure why.

int main(){
	platformers game(0, false, 0.0, 0.0);
	while(!game.updateAndRender()) {}
	return 0;
}