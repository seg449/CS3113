//
//  Vector.h
//  HW6
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard on 4/21/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//

#pragma once
#include <stdio.h>
#include <math.h>

class Vector {
public:
    Vector();
    Vector(float x, float y, float z = 0);
    float length();
    void normalize();
    float x;
    float y;
    float z;
};