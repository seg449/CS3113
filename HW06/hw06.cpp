/*
Sebastien Emmanuel Gerard
CS3113
HW#6 - Asteroids
*/

/*For the most part, this does work. As you saw when I worked with you on it, the only problem is that the sprites seem to just disappear when they go towards the right of the screen.
Other than that, everything else seems to work fine. I also have a huge chunk of code commented out in the FixedUpdate function. The point of the code was to make it such that whenever 
an asteroid went off screen, it would respawn with a higher velocity if it didnt already have a velocity higher than 5.0f. Due to the disappearing however, it didn't really work out the
way I would have liked so I left it alone. I figure once i can fix the issues with the sprites, everything else is easy. I also only used circle circle collision so they don't look the best.
*/

#include "asteroids.h"

int main(){
	Asteroids game(0.0f, 0.0f, false);
	while (!game.updateAndRender()){}
	return 0;
} 