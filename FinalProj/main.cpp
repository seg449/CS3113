//
//  main.cpp
//  Platformer
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard on 4/28/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//

#include <iostream>
#include "Platformer.h"

int main(int argc, const char * argv[]) {

    Platformer game;
 
    while (!game.UpdateAndRender()) {}

    return 0;
}
